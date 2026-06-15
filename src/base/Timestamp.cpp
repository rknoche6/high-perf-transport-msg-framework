#include "highperf/base/Timestamp.h"

#include <chrono>
#include <cinttypes>
#include <cstdio>

namespace highperf_transport {

Timestamp Timestamp::now() {
    auto since = std::chrono::system_clock::now().time_since_epoch();
    auto microsSinceEpoch =
        std::chrono::duration_cast<std::chrono::microseconds>(since).count();
    return Timestamp(static_cast<int64_t>(microsSinceEpoch));
}

std::string Timestamp::toString() const {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64,
                  micros_ / microsPerSecond, micros_ % microsPerSecond);
    return buf;
}

}
