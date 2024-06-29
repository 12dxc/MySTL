#pragma once

#include <cstddef> // size_t
#include <stdexcept> // std::runtime_error
#include <string> // std::to_string

template<typename T, size_t N>
class Array
{
public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = T const*;

    T elements_[N];

    T& operator[](size_t i) noexcept
    {
        return elements_[i];
    }

    // 适用需要修饰const的重载版本
    T const& operator[](size_t i) const noexcept
    {
        return elements_[i];
    }

    // 更安全的数组访问
    T& at(size_t i)
    {
        if (i >= N) [[unlikely]]  // 指明是小概率事件
            throw std::runtime_error("out of range! + i = " + std::to_string(i));

            return elements_[i];
    }

    T const& at(size_t i) const
    {
        if (i >= N) [[unlikely]]
            throw std::runtime_error("out of range! + i = " + std::to_string(i));

            return elements_[i];
    }

    // 往数组填充指定元素
    void fill(T const& val)
    {
        for (size_t i = 0; i != N; ++i)
            elements_[i] = val;
    }

    void swap(Array& that)
    {
        for (size_t i = 0; i != N; ++i)
            std::swap(elements_[i], that[i]);
    }

    // 返回头元素
    T& front() noexcept
    {
        return elements_[0];
    }

    // 返回尾元素
    T& back() noexcept
    {
        return elements_[N - 1];
    }

    constexpr bool empty() const noexcept
    {
        return false;
    }

    // 获取数组大小
    constexpr size_t size() const noexcept
    {
        return N;
    }

    constexpr size_t max_size() const noexcept
    {
        return N;
    }

    T const* data()const noexcept
    {
        return elements_;
    }

    // 迭代器支持
    T* begin() noexcept
    {
        return elements_;
    }
    T* end() noexcept
    {
        return elements_ + N;
    }
    T const* begin() const noexcept
    {
        return elements_;
    }
    T const* end() const noexcept
    {
        return elements_ + N;
    }
};

#if defined(_MSC_VER)
#define _LIBPENGCXX_UNREACHABLE() __assume(0)
#elif defined(__clang__)
#define _LIBPENGCXX_UNREACHABLE() __builtin_unreachable()
#elif defined(__GNUC__)
#define _LIBPENGCXX_UNREACHABLE() __builtin_unreachable()
#else
#define _LIBPENGCXX_UNREACHABLE() do {} while (1)
#endif

// 特化版本，零长度数组
template<typename T>
class Array<T, 0>
{
public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = T const*;

    T& operator[](size_t i) noexcept
    {
        _LIBPENGCXX_UNREACHABLE();
    }

    // 适用需要修饰const的重载版本
    T const& operator[](size_t i) const noexcept
    {
        _LIBPENGCXX_UNREACHABLE();
    }

    // 更安全的数组访问
    T& at(size_t i)
    {
        throw std::runtime_error("out of range! + i = " + std::to_string(i));
    }

    T const& at(size_t i) const
    {
        throw std::runtime_error("out of range! + i = " + std::to_string(i));
    }

    // 往数组填充指定元素
    void fill(T const& val)
    {
    }

    void swap(Array& that)
    {
    }

    // 返回头元素
    T& front() noexcept
    {
        _LIBPENGCXX_UNREACHABLE();
    }

    // 返回尾元素
    T& back() noexcept
    {
        _LIBPENGCXX_UNREACHABLE();
    }

    constexpr bool empty() const noexcept
    {
        return true;
    }

    // 获取数组大小
    constexpr size_t size() const noexcept
    {
        return 0;
    }

    constexpr size_t max_size() const noexcept
    {
        return 0;
    }

    T const* data()const noexcept
    {
        return nullptr;
    }

    // 迭代器支持
    T* begin() noexcept
    {
        return nullptr;
    }
    T* end() noexcept
    {
        return nullptr;
    }
    T const* begin() const noexcept
    {
        return nullptr;
    }
    T const* end() const noexcept
    {
        return nullptr;
    }
};

// 支持初始化列表
template<typename T, typename... Ts>
Array(T, Ts...) -> Array<T, 1 + sizeof...(Ts)>;
