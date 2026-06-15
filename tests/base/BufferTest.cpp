#include "highperf/base/Buffer.h"

#include <cstdio>
#include <cstring>
#include <string>

using highperf_transport::Buffer;

namespace {
int g_failures = 0;
void check(bool ok, const char* expr, int line) {
    if (!ok) {
        std::printf("FAIL line %d: %s\n", line, expr);
        ++g_failures;
    }
}
bool bytesEq(const char* p, const char* expect, size_t n) {
    return p != nullptr && std::memcmp(p, expect, n) == 0;
}
}

#define CHECK(cond) check((cond), #cond, __LINE__)

int main() {
    // fresh buffer invariants
    Buffer buf;
    CHECK(buf.readableBytes() == 0);
    CHECK(buf.writableBytes() == Buffer::kInitialSize);
    CHECK(buf.prependableBytes() == Buffer::kCheapPrepend);

    // append grows readable; peek points at the data
    buf.append("hello", 5);
    CHECK(buf.readableBytes() == 5);
    CHECK(buf.writableBytes() == Buffer::kInitialSize - 5);
    CHECK(bytesEq(buf.peek(), "hello", 5));

    // retrieve advances the reader and preserves the remainder
    buf.retrieve(2);
    CHECK(buf.readableBytes() == 3);
    CHECK(bytesEq(buf.peek(), "llo", 3));
    CHECK(buf.prependableBytes() == Buffer::kCheapPrepend + 2);

    // retrieveAsString consumes
    CHECK(buf.retrieveAsString(3) == "llo");
    CHECK(buf.readableBytes() == 0);

    // retrieveAll resets both indices
    buf.append("xy", 2);
    buf.retrieveAll();
    CHECK(buf.readableBytes() == 0);
    CHECK(buf.prependableBytes() == Buffer::kCheapPrepend);
    CHECK(buf.writableBytes() == Buffer::kInitialSize);

    // string_view append + retrieveAllAsString
    buf.append(std::string_view("abc"));
    CHECK(buf.readableBytes() == 3);
    CHECK(buf.retrieveAllAsString() == "abc");

    // growth past initial capacity keeps data intact
    Buffer big;
    std::string payload(2000, 'z');
    big.append(payload.data(), payload.size());
    CHECK(big.readableBytes() == 2000);
    CHECK(big.retrieveAllAsString() == payload);

    // prepend writes into the cheap region and rewinds the reader
    Buffer pb;
    pb.append("BODY", 4);
    size_t before = pb.prependableBytes();
    pb.prepend("HEAD", 4);
    CHECK(pb.readableBytes() == 8);
    CHECK(pb.prependableBytes() == before - 4);
    CHECK(bytesEq(pb.peek(), "HEADBODY", 8));

    // compaction: reclaim consumed prepend space instead of growing
    Buffer cb(64);
    std::string a40(40, 'a');
    cb.append(a40.data(), 40);
    cb.retrieve(30);
    std::string b30(30, 'b');
    cb.append(b30.data(), 30);
    CHECK(cb.readableBytes() == 40);
    CHECK(cb.retrieveAllAsString() == std::string(10, 'a') + std::string(30, 'b'));

    if (g_failures == 0) {
        std::printf("All Buffer tests passed\n");
        return 0;
    }
    std::printf("%d Buffer test(s) failed\n", g_failures);
    return 1;
}
