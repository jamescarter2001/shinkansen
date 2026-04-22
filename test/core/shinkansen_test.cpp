//
// Created by James on 22/04/2026.
//

#include <gtest/gtest.h>

#include "core/shinkansen.h"

struct TestMessage {
    long value;
};

TEST(Shinkansen, Initializes) {
    auto handler = [](TestMessage *, const long long) {
        // no-op
    };
    sk::shinkansen<TestMessage> underTest(1024, handler);

    EXPECT_EQ(underTest.next(), 0);
}

TEST(Shinkansen, PublishesMessage) {
    auto handler = [](const TestMessage *event, const long long seq) {
        EXPECT_EQ(seq, 0);
        EXPECT_EQ(event->value, 40);
    };
    sk::shinkansen<TestMessage> underTest(1024, handler);
    underTest.start();

    const long long seq = underTest.next();
    TestMessage* entry = underTest.get(seq);
    entry->value = 40;
    underTest.publish(seq);

    underTest.stop();
}

TEST(Shinkansen, ConsumesCommittedMessagesOnly) {
    std::atomic_long lastSeq{0};
    auto handler = [&lastSeq](const TestMessage *, const long long seq) {
        lastSeq.store(seq, std::memory_order_release);
    };
    sk::shinkansen<TestMessage> underTest(1024, handler);
    underTest.start();

    // seq = 0
    const long long seq1 = underTest.next();
    TestMessage* entry1 = underTest.get(seq1);
    entry1->value = 20;
    underTest.publish(seq1);

    // seq = 1
    const long long seq2 = underTest.next();
    TestMessage* entry2 = underTest.get(seq2);
    entry2->value = 40;
    underTest.publish(seq2);

    // seq = 2
    const long long seq3 = underTest.next();
    TestMessage* entry3 = underTest.get(seq3);
    entry3->value = 60;

    underTest.stop();

    EXPECT_EQ(lastSeq.load(std::memory_order_acquire), 1);
}