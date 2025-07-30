#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <iostream>

#define panic_string(msg)
#define panic_nostring()
#define panic_strip(msg, ...) msg
#define panic(...)	_panic(__FILE__, __func__, __LINE__, "" __VA_ARGS__)

inline void _panic(const char *file, const char *func, int line, const char *msg)
{
	std::cout << file << ":" << line << ": panicked in function " << func << ": " << msg;
	abort();
}

template<typename T>
struct Slice {
    T *ptr;
    size_t len;

    Slice(T *ptr, size_t len) : ptr(ptr), len(len) {}

    T* begin() const { return ptr; }
    T* end() const { return ptr + len; }
};


template<typename T>
struct Matrix {
    std::vector<T> data;
    size_t rows;
    size_t columns;

    Matrix(size_t r, size_t c) : data(r * c), rows(r), columns(c) {}

    void resize(size_t new_rows, size_t new_columns)
    {
        rows = new_rows;
        columns = new_columns;
        data.resize(rows * columns);
    }

    struct Row {
        T* row_data;
        size_t columns;

        T& operator[](size_t col)
        {
            if (col >= columns)
                throw std::out_of_range("Column index out of range");
            return row_data[col];
        }

        const T& operator[](size_t col) const
        {
            if (col >= columns)
                throw std::out_of_range("Column index out of range");
            return row_data[col];
        }
    };

    Row operator[](size_t row)
    {
        if (row >= rows)
            throw std::out_of_range("Row index out of range");
        return Row{ &data[row * columns], columns };
    }

    const Row operator[](size_t row) const
    {
        if (row >= rows)
            throw std::out_of_range("Row index out of range");
        return Row{ &data[row * columns], columns };
    }
};
