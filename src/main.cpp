#include <skpch.h>

#include "core/shinkansen.h"

struct Test {
    long value;
};

int main() {
    printf("hello\n");
    auto handler = [](Test* event, int seq) {
        long value = event->value;
        if ((value & (value-1)) == 0) {
            printf("%ld\n", value);
        }
    };
    sk::shinkansen<Test, decltype(handler)> test(1024, handler);
    test.start();

    long x = 0;
    while (true) {
        long long seq = test.next();
        Test* entry = test.get(seq);
        entry->value = x++;
        test.publish(seq);
    }

    return 0;
}
