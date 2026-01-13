#pragma once

#include "RefPtr.h"
#include "Weakable.h"

namespace Core
{
    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() = default;

        WeakPtr(T* ptr)
        {
            if (ptr)
                _link = ptr->_link;
        }

        WeakPtr(RefPtr<T> ptr)
        {
            if (ptr)
                _link = ptr->_link;
        }

        RefPtr<T> strong()
        {
            if (_link)
                return _link->strong<T>();
            return nullptr;
        }
    private:
        RefPtr<WeakLink> _link {};
    };
}

using Core::WeakPtr;
