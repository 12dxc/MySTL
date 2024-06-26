#pragma once

#include <cstdio>
#include <utility>
#include <concepts>

template <typename T>
struct DefaultDeleter // 默认使用 delete 释放内存
{
    void operator()(T *p) const
    {
        delete p;
    }
};

template <typename T>
struct DefaultDeleter<T[]> // 偏特化
{
    void operator()(T *p) const
    {
        delete[] p;
    }
};

// 替换值
template <typename T, typename U>
T exchange(T &dst, U &&val)
{
    T tmp = std::move(dst);
    dst = std::forward<U>(val);
    return tmp;
}

// 独占式智能指针
template <typename T, typename Deleter = DefaultDeleter<T>>
class UniquePtr
{
private:
    T *p_;

    // 让其可以访问所有的实例化对象
    template <typename U, typename UDeleter>
    friend class UniquePtr;

public: // 默认方法
    // 默认构造函数
    UniquePtr(std::nullptr_t dummy = nullptr)
    {
        p_ = nullptr;
    }

    // 自定义构造函数
    explicit UniquePtr(T *p)
    {
        p_ = p;
    }

    // 基类转子类实现动态绑定
    template <typename U, typename UDeleter>
        requires(std::convertible_to<U *, T *>)
    UniquePtr(UniquePtr<U, UDeleter> &&that) // 从子类型U的智能指针转换到T类型的智能指针
    {
        p_ = exchange(that.p_, nullptr);
    }

    // 析构函数
    ~UniquePtr()
    {
        if (p_)
            Deleter{}(p_);
    }

    /* 独占式智能指针不可拷贝 */
    // 拷贝构造函数
    UniquePtr(UniquePtr const &that) = delete;
    // 拷贝赋值函数
    UniquePtr &operator=(UniquePtr const &that) = delete;

    // 移动构造函数
    UniquePtr(UniquePtr &&that)
    {
        p_ = exchange(that.p_, nullptr);
    }

    // 移动赋值函数
    UniquePtr &operator=(UniquePtr &&that)
    {
        if (this != &that)
            [[likely]] // 更可能发生的if进行优化
        {
            if (p_)
                Deleter{}(p_);
            p_ = exchange(that.p_, nullptr);
        }
        return *this;
    }

public: // 自定义方法
    T *get() const
    {
        return p_;
    }

    // 重置指针
    T *release()
    {
        return exchange(p_, nullptr);
    }

    // 给智能指针赋新值
    void reset(T *p = nullptr)
    {
        if (p_)
            Deleter{}(p_);
        p_ = p;
    }

    T &operator*() const
    {
        return *p_;
    }

    T *operator->() const
    {
        return p_;
    }
};

// 针对数组的特化
template <class T, class Deleter>
class UniquePtr<T[], Deleter> : UniquePtr<T, Deleter>
{
};

// unique_ptr构造器
template <typename T, typename... Args>
UniquePtr<T> makeUnique(Args &&...args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

// unique_ptr构造器 -> 执行默认构造
template <class T>
UniquePtr<T> makeUniqueForOverwrite()
{
    return UniquePtr<T>(new T);
}
