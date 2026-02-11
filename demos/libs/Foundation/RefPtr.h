#pragma once

namespace Foundation
{
    template<typename T>
    class RefPtr
    {
        friend RefPtr adoptRef<T>(T*);

    public:
        RefPtr() = default;

        RefPtr(T* ptr) : _ptr { ptr }
        {
            if (_ptr)
                _ptr->ref();
        }

        template<typename U>
        RefPtr(U* ptr) : _ptr { static_cast<T*>(ptr) }
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(const RefPtr& other) : _ptr { other.get() }
        {
            if (_ptr)
                _ptr->ref();
        }

        template<typename U>
        RefPtr(const RefPtr<U>& other) : _ptr { static_cast<T*>(other.get()) }
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(RefPtr&& other) : _ptr { other.leakRef() } {}

        template<typename U>
        RefPtr(RefPtr<U>&& other) : _ptr { static_cast<T*>(other.leakRef()) } {}

        ~RefPtr()
        {
            if (_ptr)
                _ptr->unref();
        }

        RefPtr& operator=(T* ptr)
        {
            if (_ptr != ptr)
            {
                if (_ptr)
                    _ptr->unref();
                _ptr = ptr;
                if (_ptr)
                    _ptr->ref();
            }
            return *this;
        }

        RefPtr& operator=(const RefPtr& other)
        {
            RefPtr tmp { other };
            swap(tmp);
            return *this;
        }

        template<typename U>
        RefPtr& operator=(const RefPtr<U>& other)
        {
            RefPtr tmp { other };
            swap(tmp);
            return *this;
        }

        RefPtr& operator=(RefPtr&& other)
        {
            RefPtr tmp { std::move(other) };
            swap(tmp);
            return *this;
        }

        template<typename U>
        RefPtr& operator=(RefPtr<U>&& other)
        {
            RefPtr tmp { std::move(other) };
            swap(tmp);
            return *this;
        }

        T* get() const { return _ptr; }
        T* operator->() const { return _ptr; }

        operator bool() const { return _ptr; }

        [[nodiscard]] T* leakRef()
        {
            T* ptr = _ptr;
            _ptr = nullptr;
            return ptr;
        }

        void swap(RefPtr& other)
        {
            std::swap(_ptr, other._ptr);
        }

    private:
        enum AdoptTag { Adopt };

        RefPtr(AdoptTag, T* ptr) : _ptr { ptr } {}

        T* _ptr {};
    };

    template<typename T>
    RefPtr<T> adoptRef(T* ptr)
    {
        return RefPtr<T>(RefPtr<T>::Adopt, ptr);
    }
}

using Foundation::RefPtr;
using Foundation::adoptRef;
