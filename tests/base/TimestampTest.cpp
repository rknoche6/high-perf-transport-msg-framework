#include "highperf/base/Timestamp.h"

#include <cstdint>
#include <cstdio>
#include <string>

using highperf_transport::addSeconds;
using highperf_transport::Timestamp;

namespace {

int g_failures = 0;

void check(bool ok, const char* expr, int line) {
    if (!ok) {
        std::printf("FAIL line %d: %s\n", line, expr);
        ++g_failures;
    }
}

}

#define CHECK(cond) check((cond), #cond, __LINE__)

int main() {
    CHECK(!Timestamp().valid());
    CHECK(Timestamp().microsSinceEpoch() == 0);

    Timestamp a = Timestamp::now();
    Timestamp b = Timestamp::now();
    CHECK(a.valid());
    CHECK(a <= b);

    CHECK(Timestamp(1'000'000).secondsSinceEpoch() == 1);
    CHECK(Timestamp(1'000'000).secondsTo(Timestamp(2'500'000)) == 1.5);

    struct AddCase {
        int64_t base;
        double seconds;
        int64_t expected;
    };
    const AddCase addCases[] = {
        {1'000'000, 1.5, 2'500'000},
        {0, 1.0, 1'000'000},
        {1'000'000, -0.5, 500'000},
    };
    for (const AddCase& c : addCases) {
        CHECK(addSeconds(Timestamp(c.base), c.seconds).microsSinceEpoch() == c.expected);
    }

    struct StringCase {
        int64_t micros;
        const char* expected;
    };
    const StringCase stringCases[] = {
        {0, "0.000000"},
        {1'000'000, "1.000000"},
        {2'500'000, "2.500000"},
        {1'234'567, "1.234567"},
    };
    for (const StringCase& c : stringCases) {
        CHECK(Timestamp(c.micros).toString() == c.expected);
    }

    struct CompareCase {
        int64_t a;
        int64_t b;
        bool less;
        bool equal;
    };
    const CompareCase compareCases[] = {
        {10, 20, true, false},
        {20, 10, false, false},
        {10, 10, false, true},
    };
    for (const CompareCase& c : compareCases) {
        Timestamp x(c.a);
        Timestamp y(c.b);
        CHECK((x < y) == c.less);
        CHECK((x == y) == c.equal);
        CHECK((x >= y) == !c.less);
    }

    if (g_failures == 0) {
        std::printf("All Timestamp tests passed\n");
        return 0;
    }
    std::printf("%d Timestamp test(s) failed\n", g_failures);
    return 1;
}
