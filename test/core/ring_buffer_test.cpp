//
// Created by James on 22/04/2026.
//

#include <gtest/gtest.h>

#include "core/ring_buffer.h"

TEST(Shinkansen, GetWraps) {
    sk::ring_buffer<int> underTest(8);
    *underTest.get(0) = 50;

    ASSERT_EQ(*underTest.get(0), 50);
    ASSERT_EQ(*underTest.get(8), 50);
    ASSERT_EQ(*underTest.get(16), 50);
}