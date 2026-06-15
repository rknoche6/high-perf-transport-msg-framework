#pragma once

#include <compare>
#include <cstdint>
#include <string>

namespace highperf_transport {

class Timestamp {
public:
    static constexpr int64_t microsPerSecond = 1000 * 1000;

    Timestamp() = default;
    explicit Timestamp(int64_t microsSinceEpoch) : micros_(microsSinceEpoch) {}

    static Timestamp now();
    static Timestamp invalid() { return Timestamp(); }

    bool valid() const { return micros_ > 0; }
    int64_t microsSinceEpoch() const { return micros_; }
    int64_t secondsSinceEpoch() const { return micros_ / microsPerSecond; }

    double secondsTo(Timestamp other) const {
        return static_cast<double>(other.micros_ - micros_) / microsPerSecond;
    }

    std::string toString() const;

    auto operator<=>(const Timestamp&) const = default;

private:
    int64_t micros_ = 0;
};

inline Timestamp addSeconds(Timestamp t, double seconds) {
    return Timestamp(t.microsSinceEpoch() +
                     static_cast<int64_t>(seconds * Timestamp::microsPerSecond));
}

}
