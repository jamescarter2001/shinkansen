#include <skpch.h>

#include "core/shinkansen.h"

struct Test {
    long value;
    bool another;
    int andAnother;
};

int main() {
    auto handler = [](Test* event, long seq) {
        long value = event->value;
        if ((value & (value-1)) == 0) {
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

    return 0;
}
