#include "highperf/base/Buffer.h"

#include <algorithm>
#include <cstring>

namespace highperf_transport {

Buffer::Buffer(size_t initialSize)
    : buffer_(kCheapPrepend + initialSize),
      readerIndex_(kCheapPrepend),
      writerIndex_(kCheapPrepend) {}

size_t Buffer::readableBytes() const {
    return writerIndex_ - readerIndex_;
}

size_t Buffer::writableBytes() const {
    return buffer_.size() - writerIndex_;
}

size_t Buffer::prependableBytes() const {
    return readerIndex_;
}

const char* Buffer::peek() const {
    return begin() + readerIndex_;
}

void Buffer::retrieve(size_t len) {
    if (len < readableBytes()) {
        readerIndex_ += len;
    } else {
        retrieveAll();
    }
}

void Buffer::retrieveAll() {
    readerIndex_ = kCheapPrepend;
    writerIndex_ = kCheapPrepend;
}

std::string Buffer::retrieveAsString(size_t len) {
    std::string result(peek(), len);
    retrieve(len);
    return result;
}

std::string Buffer::retrieveAllAsString() {
    return retrieveAsString(readableBytes());
}

void Buffer::append(const char* data, size_t len) {
    ensureWritableBytes(len);
    std::memcpy(beginWrite(), data, len);
    hasWritten(len);
}

void Buffer::append(std::string_view data) {
    append(data.data(), data.size());
}

void Buffer::ensureWritableBytes(size_t len) {
    if (writableBytes() < len) {
        makeSpace(len);
    }
}

char* Buffer::beginWrite() {
    return begin() + writerIndex_;
}

const char* Buffer::beginWrite() const {
    return begin() + writerIndex_;
}

void Buffer::hasWritten(size_t len) {
    writerIndex_ += len;
}

void Buffer::prepend(const void* data, size_t len) {
    readerIndex_ -= len;
    std::memcpy(begin() + readerIndex_, data, len);
}

char* Buffer::begin() {
    return buffer_.data();
}

const char* Buffer::begin() const {
    return buffer_.data();
}

void Buffer::makeSpace(size_t len) {
    if (writableBytes() + prependableBytes() - kCheapPrepend < len) {
        buffer_.resize(writerIndex_ + len);
    } else {
        size_t readable = readableBytes();
        std::copy(begin() + readerIndex_, begin() + writerIndex_, begin() + kCheapPrepend);
        readerIndex_ = kCheapPrepend;
        writerIndex_ = readerIndex_ + readable;
    }
}

}
