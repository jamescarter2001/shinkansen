//
// Created by James on 20/04/2026.
//

#ifndef SHINKANSEN_MATH_H
#define SHINKANSEN_MATH_H

namespace sk {
    static bool is_power_of_two(const unsigned int num) {
        return std::has_single_bit(num);
    }
}

#endif //SHINKANSEN_MATH_H