#pragma once
#include <stdexcept>
#include <memory>
#include "Common/Error.h"

namespace Daybreak
{
    /** Casts a shared pointer to a reference. Asserts if the pointer is null when compiled in debug mode. */
    template<typename T>
    inline constexpr T& deref(const std::shared_ptr<T>& ptr) noexcept
    {
        return deref(ptr.get());
    }

    /** Casts a shared pointer to a reference and will throw an exception if the pointer is null. */
    template<typename T>
    inline T& deref_checked(const std::shared_ptr<T>& ptr)
    {
        return deref_checked(ptr.get());
    }

    /** Casts a shared pointer to a reference without checking if it is null. */
    template<typename T>
    inline constexpr T& deref_unchecked(const std::shared_ptr<T>& ptr) noexcept
    {
        return deref_unchecked(ptr.get());
    }

    /** Casts a shared pointer to a reference. Asserts if the pointer is null when compiled in debug mode. */
    template<typename T>
    inline constexpr const T& deref(const std::shared_ptr<const T>& ptr) noexcept
    {
        return deref(ptr.get());
    }

    /** Casts a shared pointer to a reference and will throw an exception if the pointer is null. */
    template<typename T>
    inline const T& deref_checked(const std::shared_ptr<const T>& ptr)
    {
        return deref_checked(ptr.get());
    }

    /** Casts a shared pointer to a reference without checking if it is null. */
    template<typename T>
    inline constexpr const T& deref_unchecked(const std::shared_ptr<const T>& ptr) noexcept
    {
        return deref_unchecked(ptr.get());
    }

    /** Casts a unique pointer to a reference. Asserts if the pointer is null when compiled in debug mode. */
    template<typename T>
    inline constexpr T& deref(const std::unique_ptr<T>& ptr) noexcept
    {
        return deref(ptr.get());
    }

    /** Casts a unique pointer to a reference and will throw an exception if the pointer is null. */
    template<typename T>
    inline T& deref_checked(const std::unique_ptr<T>& ptr)
    {
        return deref_checked(ptr.get());
    }

    /** Casts a unique pointer to a reference without checking if it is null. */
    template<typename T>
    inline constexpr T& deref_unchecked(const std::unique_ptr<T>& ptr) noexcept
    {
        return deref_unchecked(ptr.get());
    }

    /** Casts a pointer to a reference. Asserts if the pointer is null when compiled in debug mode. */
    template<typename T>
    inline constexpr const T& deref(const std::unique_ptr<const T>& ptr) noexcept
    {
        return deref(ptr.get());
    }

    /** Casts a unique pointer to a reference and will throw an exception if the pointer is null. */
    template<typename T>
    inline const T& deref_checked(const std::unique_ptr<const T>& ptr)
    {
        return deref_checked(ptr.get());
    }

    /** Casts a shared pointer to a reference without checking if it is null. */
    template<typename T>
    inline constexpr const T& deref_unchecked(const std::unique_ptr<const T>& ptr) noexcept
    {
        return deref_unchecked(ptr.get());
    }

    /** Casts a pointer to a reference. Asserts if the pointer is null when compiled in debug mode. */
    template<typename T>
    constexpr inline T& deref(T* ptr) noexcept
    {
        EXPECT(ptr != nullptr, "Cannot convert null pointer to reference");
        return *ptr;
    }

    /** Casts a pointer to a reference and will throw an exception if the pointer is null. */
    template<typename T>
    inline T& deref_checked(T* ptr)
    {
        if (ptr == nullptr)
        {
            throw std::invalid_argument("Cannot convert null pointer to reference");
        }

        return *ptr;
    }

    /** Casts a unique pointer to a reference without checking if it is null. */
    template<typename T>
    inline constexpr T& deref_unchecked(T* ptr) noexcept
    {
        return *ptr;
    }

    /** Casts a pointer to a reference. Asserts if the pointer is null when compiled in debug mode. */
    template<typename T>
    constexpr const T& deref(const T* ptr) noexcept
    {
        EXPECT(ptr != nullptr, "Cannot convert null pointer to reference");
        return *ptr;
    }

    /** Casts a pointer to a reference and will throw an exception if the pointer is null. */
    template<typename T>
    inline const T& deref_checked(const T* ptr)
    {
        if (ptr == nullptr)
        {
            throw std::invalid_argument("Cannot convert null pointer to reference");
        }

        return *ptr;
    }

    /** Casts a unique pointer to a reference without checking if it is null. */
    template<typename T>
    inline constexpr const T& deref_unchecked(const T* ptr) noexcept
    {
        return *ptr;
    }
}