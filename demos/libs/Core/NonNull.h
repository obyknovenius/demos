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
        NonNull(const NonNull<U>& other) : _ptr { static_cast<T>(other.get()) }
        {
        }

        T get() const { return _ptr; }
        operator T() const { return _ptr; }
        T operator->() const { return _ptr; }

        NonNull(std::nullptr_t) = delete;
        NonNull& operator=(std::nullptr_t) = delete;

    private:
        T _ptr;
    };
}

using Core::NonNull;
