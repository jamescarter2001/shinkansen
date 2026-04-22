//
// Created by James on 20/04/2026.
//

#ifndef SHINKANSEN_RING_BUFFER_H
#define SHINKANSEN_RING_BUFFER_H

#include <skpch.h>

#include "sk_math.h"

namespace sk {
    template<typename T>
    class ring_buffer {
    public:
        ring_buffer(const unsigned int size) : m_size(size), m_mask(size-1), m_buffer(size) {
            if (!is_power_of_two(size)) {
                throw std::runtime_error("ring buffer size must be a power of two.");
            }

            for (int i = 0; i < size; i++) {
                m_buffer[i] = T{0};
            }
        }
        ~ring_buffer() = default;

        T* get(const long long index) {
            return &m_buffer[index & m_mask];
        }

        [[nodiscard]] int get_size() const {
            return m_size;
        }
    private:
        const unsigned int m_size;
        const unsigned int m_mask;
        std::vector<T> m_buffer;
    };
} // sk

#endif //SHINKANSEN_RING_BUFFER_H