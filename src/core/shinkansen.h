//
// Created by James on 21/04/2026.
//

#ifndef SHINKANSEN_SHINKANSEN_H
#define SHINKANSEN_SHINKANSEN_H

#include "ring_buffer.h"

namespace sk {
    template<typename T>
    class shinkansen {
    public:
        shinkansen(int size, std::function<void(T*, long long)> func) : m_size(size), m_ringBuffer(size), m_consumerFunc(func) {}
        ~shinkansen() {
            stop();
        }

        void start() {
            if (m_consumerThread.joinable()) {
                throw std::runtime_error("Already started!");
            }
            m_running.store(true, std::memory_order_relaxed);
            m_consumerThread = std::jthread(&shinkansen::consumeLoop, this);
        }

        void stop() {
            bool expected = true;
            if (m_running.compare_exchange_strong(expected, false, std::memory_order_release, std::memory_order_relaxed)) {
                m_consumerThread.join();
            }
        }

        long long next() {
            const long long nextSeq = m_consumer.m_nextSequence+1;

            const long long wrapPoint = nextSeq - m_size;

            if (wrapPoint > m_consumer.m_cachedConsumerSeq) {
                long long newConsumerSeq = m_consumerSeq.load(std::memory_order_acquire);
                while (wrapPoint > newConsumerSeq) {
                    newConsumerSeq = m_consumerSeq.load(std::memory_order_acquire);
                }
                m_consumer.m_cachedConsumerSeq = newConsumerSeq;
            }
            m_consumer.m_nextSequence = nextSeq;

            return nextSeq;
        }

        void publish(const long long seq) {
            m_producerSeq.store(seq, std::memory_order_release);
        }

        void consumeLoop() {
            long long currentSeq = m_consumerSeq.load(std::memory_order_relaxed);
            while ( m_running.load(std::memory_order_acquire)) {
                const long long producerSeq = m_producerSeq.load(std::memory_order_acquire);

                if (currentSeq == producerSeq) {
                    continue;
                }

                while (currentSeq < producerSeq) {
                    T* data = m_ringBuffer.get(++currentSeq);
                    m_consumerFunc(data, currentSeq);
                }

                m_consumerSeq.store(currentSeq, std::memory_order_release);
            }

            // drain the rest of the messages before exiting.
            const long long producerSeq = m_producerSeq.load(std::memory_order_acquire);
            while (currentSeq < producerSeq) {
                T* data = m_ringBuffer.get(++currentSeq);
                m_consumerFunc(data, currentSeq);
            }
        }

        T* get(unsigned long long seq) {
            return m_ringBuffer.get(seq);
        }
    private:
        const unsigned int m_size;
        alignas(128) std::atomic_bool m_running{false};

        alignas(128) struct PublishVars {
            long long m_cachedConsumerSeq = -1;
            long long m_nextSequence = -1;
        } m_consumer;

        alignas(128) std::atomic_llong m_producerSeq = -1;
        alignas(128) std::atomic_llong m_consumerSeq = -1;

        alignas(128) ring_buffer<T> m_ringBuffer;
        const std::function<void(T*, long long)> m_consumerFunc;
        std::jthread m_consumerThread;
    };
}

#endif //SHINKANSEN_SHINKANSEN_H