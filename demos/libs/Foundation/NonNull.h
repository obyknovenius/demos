#pragma once

#include <cassert>
#include <concepts>

namespace Foundation
{
    template <typename T>
    class NonNull
    {
    public:
        NonNull(T ptr) : _ptr { ptr }
        {
            assert(ptr != nullptr);
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        NonNull(U ptr) : _ptr { ptr }
        {
            assert(ptr != nullptr);
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        NonNull(const NonNull<U>& other) : _ptr { other.get() }
        {
            assert(_ptr != nullptr);
        }

        T get() const { return _ptr; }

        operator T() const { return get(); }
        T operator->() const { return get(); }
        T& operator*() const { return *get(); }

        NonNull(std::nullptr_t) = delete;
        NonNull& operator=(std::nullptr_t) = delete;

    private:
        T _ptr;
    };
}

using Foundation::NonNull;
