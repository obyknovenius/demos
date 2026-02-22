#pragma once

#include "RefCounted.h"
#include "RefPtr.h"

namespace Foundation
{
    template<typename T>
    class WeakPtr;

    template<typename T>
    class WeakLink final : public RefCounted
    {
    public:
        WeakLink(T* ptr) : _ptr { ptr } {}
        ~WeakLink() = default;

        T* get() const { return _ptr; }

        void revoke() { _ptr = nullptr; }
    private:
        T* _ptr {};

    };

    template<typename T>
    class EnableWeakPtr
    {
        friend class WeakPtr<T>;

    public:
        EnableWeakPtr() : _weakLink { new WeakLink(static_cast<T*>(this)) } {}

        ~EnableWeakPtr()
        {
            _weakLink->revoke();
        }

    private:
        RefPtr<WeakLink<T>> _weakLink;
    };
}

using Foundation::EnableWeakPtr;
