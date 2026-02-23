#pragma once

#include "EnableWeakPtr.h"

namespace Foundation
{
    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() = default;

        WeakPtr(T* ptr) : _weakLink { ptr ? static_cast<EnableWeakPtr<T>*>(ptr)->weakLink() : nullptr } {}

        WeakPtr& operator=(T* ptr)
        {
            _weakLink = ptr ? static_cast<EnableWeakPtr<T>*>(ptr)->weakLink() : nullptr;
            return *this;
        }

        T* get() const
        {
            if (_weakLink)
                return static_cast<T*>(_weakLink->get());
            return nullptr;
        }

    private:
        RefPtr<WeakLink<T>> _weakLink;
    };
}

using Foundation::WeakPtr;
