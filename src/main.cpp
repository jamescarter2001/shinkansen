#include <skpch.h>

#include "core/shinkansen.h"

struct Test {
    long value;
    bool another;
    int andAnother;
};

int main() {
    auto handler = [](const Test* event, const long long seq) {
        long value = event->value;
        if (sk::is_power_of_two(value)) {
            printf("%ld\n", value);
        }
    };
    sk::shinkansen<Test> test(1024, handler);
    test.start();

    for (long i = 0; i <= 268435456; i++) {
        const long long seq = test.next();
        Test* entry = test.get(seq);
        entry->value = i;
        entry->another = true;
        entry->andAnother = 40;
        test.publish(seq);
    }

    test.stop();

    return 0;
}
