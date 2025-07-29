#pragma once

#include <cstddef>

template<typename T>
struct Slice {
    T *ptr;
    size_t len;

    Slice(T *ptr, size_t len) : ptr(ptr), len(len) {}

    T* begin() const { return ptr; }
    T* end() const { return ptr + len; }
};
