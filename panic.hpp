#pragma once

#include <iostream>

#define panic_string(msg)
#define panic_nostring()
#define panic_strip(msg, ...) msg
#define panic(...)	panic_strip(,##__VA_ARGS__, panic_string(__VA_ARGS__), panic_nostring(__VA_ARGS__))

inline void _panic(const char *file, const char *func, int line, const char *msg)
{
	std::cout << file << ":" << line << ": panicked in function " << func << ": " << msg;
	abort();
}
