#pragma once

#include "NonnullRefPtr.h"

namespace Core
{
    template <typename T>
    class RefPtr
    {
    public:
        enum AdoptTag { adopt };

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

        bool operator==(const RefPtr& other) const { return ptr() == other.ptr(); }
        bool operator!=(const RefPtr& other) const { return ptr() != other.ptr(); }

        template<typename U>
        bool operator==(const NonnullRefPtr<U>& other) const { return ptr() == other.ptr(); }

        bool operator==(T other) const { return ptr() == other; }

    private:
        T* _ptr { nullptr };
    };

    template<typename T>
    RefPtr<T> adopt(T* ptr)
    {
        return RefPtr<T>(RefPtr<T>::adopt, ptr);
    }
}

using Core::RefPtr;
using Core::adopt;
