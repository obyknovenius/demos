#pragma once

#include <cassert>

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
