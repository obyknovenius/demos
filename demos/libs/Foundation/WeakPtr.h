#pragma once

#include "RefPtr.h"
#include "Weakable.h"

namespace Foundation
{
    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() = default;

        template<typename U>
        WeakPtr(RefPtr<U> ptr)
        {
            if (ptr)
                _weakLink = static_cast<Weakable<T>*>(ptr.get())->weakLink();
        }

        T* get() const
        {
            if (_weakLink)
                return _weakLink->get();
            return nullptr;
        }
    private:
        RefPtr<WeakLink<T>> _weakLink {};
    };
}

using Foundation::WeakPtr;
