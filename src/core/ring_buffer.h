//
// Created by James on 20/04/2026.
//

#ifndef SHINKANSEN_RING_BUFFER_H
#define SHINKANSEN_RING_BUFFER_H

#include <skpch.h>

#include "sk_math.h"

#define PADDING 16

namespace sk {
    template<typename T>
    class ring_buffer {
    public:
        ring_buffer(const unsigned int size) : m_size(size), m_mask(size-1), m_ringBuffer(new T*[PADDING + size + PADDING]{nullptr}) {
            if (!is_power_of_two(size)) {
                throw std::runtime_error("ring buffer size must be a power of two.");
            }

            for (int i = PADDING; i < (size + PADDING); i++) {
                m_ringBuffer[i] = new T();
            }
        }
        ~ring_buffer() {
            for (int i = 0; i < m_size; i++) {
                delete m_ringBuffer[i];
            }
            delete[] m_ringBuffer;
        }

        T* get(const long long index) {
            return m_ringBuffer[PADDING + (index & m_mask)];
        }

        [[nodiscard]] int get_size() const {
            return m_size;
        }
    private:
        const unsigned int m_size;
        const unsigned int m_mask;
        T **const m_ringBuffer;
    };
} // sk

#endif //SHINKANSEN_RING_BUFFER_H