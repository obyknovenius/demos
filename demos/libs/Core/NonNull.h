#pragma once

#include <cassert>

namespace Core
{
    template <typename T>
    class NonNull
    {
    public:
        NonNull(T ptr) : _ptr { ptr }
        {
            assert(ptr);
        }

        template<typename U>
        NonNull(U ptr) : _ptr { static_cast<T>(ptr) }
        {
            assert(ptr);
        }

        template<typename U>
        NonNull(const NonNull<U>& other) : _ptr { static_cast<T>(other.get()) }
        {
        }

        T get() const { return _ptr; }
        operator T() const { return _ptr; }
        T operator->() const { return _ptr; }

        template<typename U>
        U get() const { return static_cast<U>(_ptr); }
        template<typename U>
        operator U() const { return static_cast<U>(_ptr); }
        template<typename U>
        U operator->() const { return static_cast<U>(_ptr); }

        NonNull(std::nullptr_t) = delete;
        NonNull& operator=(std::nullptr_t) = delete;

    private:
        T _ptr;
    };
}

using Core::NonNull;
