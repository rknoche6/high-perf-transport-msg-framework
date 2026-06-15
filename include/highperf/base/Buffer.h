#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace highperf_transport {

class Buffer {
public:
    static constexpr size_t kCheapPrepend = 8;
    static constexpr size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize);

    size_t readableBytes() const;
    size_t writableBytes() const;
    size_t prependableBytes() const;

    const char* peek() const;

    void retrieve(size_t len);
    void retrieveAll();
    std::string retrieveAsString(size_t len);
    std::string retrieveAllAsString();

    void append(const char* data, size_t len);
    void append(std::string_view data);

    void ensureWritableBytes(size_t len);
    char* beginWrite();
    const char* beginWrite() const;
    void hasWritten(size_t len);

    void prepend(const void* data, size_t len);

private:
    char* begin();
    const char* begin() const;
    void makeSpace(size_t len);

    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};

}
