//
// Created by James on 20/04/2026.
//

#ifndef SHINKANSEN_MATH_H
#define SHINKANSEN_MATH_H

#include <skpch.h>

namespace sk {
    static bool is_power_of_two(const size_t num) {
        return (num & (num - 1)) == 0;
    }
}

#endif //SHINKANSEN_MATH_H