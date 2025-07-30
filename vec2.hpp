#pragma once

#include <iostream>

template<typename T>
struct Vec2 {
    T x, y;

    Vec2() : x(0), y(0) {}

    Vec2(T x, T y) : x(x), y(y) {}

    using V = Vec2<T>;

    friend V operator+(const V v1, const V v2)
    {
        return Vec2(v1.x + v2.x, v1.y + v2.y);
    }

    friend V operator-(const V v1, const V v2)
    {
        return Vec2(v1.x - v2.x, v1.y - v2.y);
    }

    friend V operator*(const V v1, const V v2)
    {
        return Vec2(v1.x * v2.x, v1.y * v2.y);
    }

    friend V operator/(const V v1, const V v2)
    {
        return Vec2(v1.x / v2.x, v1.y / v2.y);
    }

    friend V operator+(const V v, const T t)
    {
        return Vec2(v.x + t, v.y + t);
    }

    friend V operator-(const V v, const T t)
    {
        return Vec2(v.x - t, v.y - t);
    }

    friend V operator*(const V v, const T t)
    {
        return Vec2(v.x * t, v.y * t);
    }

    friend V operator/(const V v, const T t)
    {
        return Vec2(v.x / t, v.y / t);
    }

    friend bool operator<(const V v, const V t)
    {
        return (v.x*v.x + v.y*v.y) < (t.x*t.x + t.y*t.y);
    }

    friend bool operator>(const V v, const V t)
    {
        return !(v < t);
    }

    V operator+=(const V v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }

    V operator-=(const V v)
    {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }

    V operator*=(const V v)
    {
        this->x *= v.x;
        this->y *= v.y;
        return *this;
    }

    V operator/=(const V v)
    {
        this->x /= v.x;
        this->y /= v.y;
        return *this;
    }

    V operator+=(const T t)
    {
        this->x += t;
        this->y += t;
        return *this;
    }

    V operator-=(const T t)
    {
        this->x -= t;
        this->y -= t;
        return *this;
    }

    V operator*=(const T t)
    {
        this->x *= t;
        this->y *= t;
        return *this;
    }

    V operator/=(const T t)
    {
        this->x /= t;
        this->y /= t;
        return *this;
    }

    V abs()
    {
        return Vec2(std::abs(this->x), std::abs(this->y));
    }

    // Rect1 = (min x, y) -> min1, (max x, y) -> max1
    // Rect2 = (min x, y) -> min2, (max x, y) -> max2
    bool overlap(const V min1, const V min2, const V max1, const V max2)
    {
        return min1.x < max2.x && min2.x < max1.x && min1.y < max2.y && min2.y < max1.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const V v)
    {
        return os << "Vec2{" << v.x << ", " << v.y << "}";
    }
};
