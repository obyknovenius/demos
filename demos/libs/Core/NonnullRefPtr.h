#pragma once

namespace Core
{
    template <typename T>
    class NonnullRefPtr
    {
    public:
        enum AdoptTag { Adopt };

        NonnullRefPtr(T& ref) : _ptr { &ref }
        {
            if (_ptr)
                _ptr->ref();
        }

        NonnullRefPtr(AdoptTag, T& ref) : _ptr { &ref }
        {
        }

        NonnullRefPtr(const NonnullRefPtr& other) : _ptr { other.ptr() }
        {
            if (_ptr)
                _ptr->ref();
        }

        template<typename U>
        NonnullRefPtr(const NonnullRefPtr<U>& other) : _ptr { static_cast<T*>(other.ptr()) }
        {
            _ptr->ref();
        }

        ~NonnullRefPtr()
        {
            if (_ptr)
                _ptr->unref();
        }

        NonnullRefPtr& operator=(const NonnullRefPtr& other)
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
    NonnullRefPtr<T> adopt(T& ref)
    {
        return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, ref);
    }
}

using Core::NonnullRefPtr;
using Core::adopt;
