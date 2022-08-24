#pragma once

#include <cmath>
#include <iostream>

#define LOG(...)                                                               \
    {                                                                          \
        ell_log::println("func: {}, line: {} ", __func__, __LINE__);               \
        ell_log::println(__VA_ARGS__);                                             \
    }

namespace ell_log {

// void dealFilePath() {}

// 匹配其他普通数据类型
template <typename T>
void cout(const T &value) {
    std::cout << value;
}

inline void println(const char *s) { std::cout << s << std::endl; }

template <typename T, typename... Args>
void println(const char *s, T value, Args... args) {
    while (*s) {
        if (*s == '{' && *++s == '}') {
            cout(value);
            return println(++s, args...);
        }
        std::cout << *s++;
    }
}

inline void print(const char *s) { std::cout << s; }

template <typename T, typename... Args>
void print(const char *s, T value, Args... args) {
    while (*s) {
        if (*s == '{' && *++s == '}') {
            cout(value);
            return print(++s, args...);
        }
        std::cout << *s++;
    }
}
} // namespace ell
