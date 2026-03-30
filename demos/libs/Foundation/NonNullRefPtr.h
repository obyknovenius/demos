#pragma once

#include <concepts>
#include <utility>

namespace Foundation
{
    template<typename T>
    class RefPtr;

    template<typename T>
    class WeakPtr;

    template<typename T>
    class NonNullRefPtr
    {
    public:
        static NonNullRefPtr adopt(T* ptr) { return NonNullRefPtr(Adopt, ptr); }

        NonNullRefPtr() = default;

        NonNullRefPtr(T* ptr) : _ptr { ptr }
        {
            if (_ptr)
                _ptr->ref();
        }

        NonNullRefPtr(const RefPtr<T>& ptr) : _ptr { ptr.get() }
        {
            if (_ptr)
                _ptr->ref();
        }

        NonNullRefPtr(const WeakPtr<T>& ptr) : _ptr { ptr.get() }
        {
            if (_ptr)
                _ptr->ref();
        }

        NonNullRefPtr(const NonNullRefPtr& other) : _ptr { other.get() }
        {
            if (_ptr)
                _ptr->ref();
        }

        template<typename U>
        requires(std::derived_from<U, T>)
        NonNullRefPtr(const NonNullRefPtr<U>& other) : _ptr { static_cast<T*>(other.get()) }
        {
            if (_ptr)
                _ptr->ref();
        }

        NonNullRefPtr(NonNullRefPtr&& other) : _ptr { other.leak() } {}

        template<typename U>
        requires(std::derived_from<U, T>)
        NonNullRefPtr(NonNullRefPtr<U>&& other) : _ptr { static_cast<T*>(other.leak()) } {}

        ~NonNullRefPtr()
        {
            if (_ptr)
                _ptr->unref();
        }

        NonNullRefPtr& operator=(T* ptr)
        {
            NonNullRefPtr tmp { ptr };
            swap(tmp);
            return *this;
        }

        NonNullRefPtr& operator=(const NonNullRefPtr& other)
        {
            NonNullRefPtr tmp { other };
            swap(tmp);
            return *this;
        }

        template<typename U>
        requires(std::derived_from<U, T>)
        NonNullRefPtr& operator=(const NonNullRefPtr<U>& other)
        {
            NonNullRefPtr tmp { other };
            swap(tmp);
            return *this;
        }

        NonNullRefPtr& operator=(NonNullRefPtr&& other)
        {
            NonNullRefPtr tmp { std::move(other) };
            swap(tmp);
            return *this;
        }

        template<typename U>
        requires(std::derived_from<U, T>)
        NonNullRefPtr& operator=(NonNullRefPtr<U>&& other)
        {
            NonNullRefPtr tmp { std::move(other) };
            swap(tmp);
            return *this;
        }

        T* get() const { return _ptr; }

        T* operator->() const { return get(); }
        T& operator*() const { return *get(); }

        bool operator==(std::nullptr_t) const { return !get(); }
        bool operator==(const NonNullRefPtr& other) const { return get() == other.get(); }

        explicit operator bool() const { return get(); }

        [[nodiscard]] T* leak()
        {
            T* ptr = _ptr;
            _ptr = nullptr;
            return ptr;
        }

        void swap(NonNullRefPtr& other)
        {
            std::swap(_ptr, other._ptr);
        }

    private:
        enum AdoptTag { Adopt };

        NonNullRefPtr(AdoptTag, T* ptr) : _ptr { ptr } {}

        T* _ptr {};
    };

    template<typename T, typename U>
    inline NonNullRefPtr<T> staticPtrCast(NonNullRefPtr<U> const& ptr)
    {
        return NonNullRefPtr<T>(static_cast<T*>(ptr.get()));
    }
}

using Foundation::NonNullRefPtr;
using Foundation::staticPtrCast;
