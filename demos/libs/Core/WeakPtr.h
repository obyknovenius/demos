#pragma once

#include "NonnullRefPtr.h"
#include "RefPtr.h"
#include "Weakable.h"

namespace Core
{
    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() = default;

        WeakPtr(const T* ptr)
        {
            if (ptr)
                _link = ptr->_weakLink;
        }

        WeakPtr(const RefPtr<T>& ptr)
        {
            if (ptr)
                _link = ptr->_weakLink;
        }

        WeakPtr(const NonnullRefPtr<T>& ptr)
        {
            _link = ptr->_weakLink;
        }

        RefPtr<T> strong()
        {
            if (_link)
                return _link->strong();
            return nullptr;
        }
    private:
        RefPtr<WeakLink> _link {};
    };
}

using Core::WeakPtr;
