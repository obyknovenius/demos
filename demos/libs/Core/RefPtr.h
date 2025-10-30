#pragma once

#include "NonnullRefPtr.h"

namespace Core
{
    template <typename T>
    class RefPtr
    {
    public:
        enum AdoptTag { Adopt };

        RefPtr(T* ptr = nullptr) : _ptr { ptr }
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(AdoptTag, T* ptr) : _ptr { ptr }
        {
        }

        RefPtr(const RefPtr& other) : _ptr { other.ptr() }
        {
            if (_ptr)
                _ptr->ref();
        }

        template<typename U>
        RefPtr(const RefPtr<U>& other) : _ptr { static_cast<T*>(other.ptr()) }
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(const NonnullRefPtr<T>& other) : _ptr { other.ptr() }
        {
            _ptr->ref();
        }

        template<typename U>
        RefPtr(const NonnullRefPtr<U>& other) : _ptr { static_cast<T*>(other.ptr()) }
        {
            _ptr->ref();
        }

        ~RefPtr()
        {
            if (_ptr)
                _ptr->unref();
        }

        RefPtr& operator=(const RefPtr& other)
        {
            if (this != &other)
            {
                if (_ptr)
                    _ptr->unref();
                _ptr = other.ptr();
                if (_ptr)
                    _ptr->ref();
            }
            return *this;
        }

        T* ptr() const { return _ptr; }
        T& operator*() const { return *_ptr; }
        T* operator->() const { return _ptr; }

        operator bool() const { return _ptr; }

    private:
        T* _ptr { nullptr };
    };

    template<typename T>
    RefPtr<T> adopt(T* ptr)
    {
        return RefPtr<T>(RefPtr<T>::Adopt, ptr);
    }
}

using Core::RefPtr;
using Core::adopt;
