#pragma once

#include <cassert>
#include <concepts>
#include <utility>

namespace Foundation
{
    template <typename T>
    class NonNull;

    template <typename T>
    requires std::is_pointer_v<T>
    class NonNull<T>
    {
    public:
        NonNull(std::nullptr_t) = delete;

        NonNull(T ptr) : _ptr(ptr)
        {
            assert(_ptr);
        }

        operator T() const { return _ptr; }

        T operator->() const { return _ptr; }

        NonNull& operator=(std::nullptr_t) = delete;

    private:
        T _ptr;
    };

    template <typename T>
    class StrongPtr;

    template <typename T>
    class NonNull<StrongPtr<T>>
    {
    public:
        NonNull(std::nullptr_t) = delete;

        template <typename... Args>
        NonNull(Args&&... args) : _ptr(std::forward<Args>(args)...)
        {
            assert(_ptr);
        }

        StrongPtr<T>& get() { return _ptr; }

        operator StrongPtr<T>() const { return _ptr; }

        T* operator->() const { return _ptr.get(); }
        T& operator*() const { return *_ptr.get(); }

        NonNull& operator=(std::nullptr_t) = delete;

    private:
        StrongPtr<T> _ptr;
    };
}

using Foundation::NonNull;
