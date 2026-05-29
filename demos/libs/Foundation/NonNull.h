#pragma once

#include <cassert>
#include <concepts>

namespace Foundation
{
    template <typename T>
    class NonNull
    {
    public:
        NonNull(std::nullptr_t) = delete;

        NonNull(T ptr) : _ptr(ptr)
        {
            assert(_ptr);
        }

        T get() const { return _ptr; }

        operator T() const { return _ptr; }
        T operator->() const { return _ptr; }
        std::remove_pointer_t<T>& operator*() const { return *_ptr; }
        
        NonNull& operator=(std::nullptr_t) = delete;

    private:
        T _ptr;
    };

    template <typename T>
    class WeakPtr;

    template <typename T>
    class RefPtr;

    template <typename T>
    class NonNull<RefPtr<T>>
    {
    public:
        NonNull(std::nullptr_t) = delete;

        NonNull(T* ptr) : _refPtr(ptr)
        {
            assert(_refPtr);
        }

        NonNull(const NonNull<T*> nonNullPtr) : _refPtr(nonNullPtr)
        {
            assert(_refPtr);
        }

        NonNull(const RefPtr<T>& refPtr) : _refPtr(refPtr)
        {
            assert(_refPtr);
        }

        template <typename U>
        requires std::derived_from<U, T>
        NonNull(const RefPtr<U>& refPtr) : _refPtr(refPtr)
        {
            assert(_refPtr);
        }

        NonNull(const NonNull<RefPtr<T>>& nonNullRefPtr) : _refPtr(nonNullRefPtr)
        {
            assert(_refPtr);
        }

        template <typename U>
        requires std::derived_from<U, T>
        NonNull(const NonNull<RefPtr<U>>& nonNullRefPtr) : _refPtr(nonNullRefPtr)
        {
            assert(_refPtr);
        }

        NonNull(const WeakPtr<T>& weakPtr) : _refPtr(weakPtr)
        {
            assert(_refPtr);
        }

        template <typename U>
        requires std::derived_from<U, T>
        NonNull(const WeakPtr<U>& weakPtr) : _refPtr(weakPtr)
        {
            assert(_refPtr);
        }

        ~NonNull() = default;

        const RefPtr<T>& get() const { return _refPtr; }

        operator RefPtr<T>() const { return _refPtr; }
        T* operator->() const { return _refPtr.get(); }
        T& operator*() const { return *_refPtr.get(); }

        NonNull& operator=(std::nullptr_t) = delete;

    private:
        RefPtr<T> _refPtr;
    };

    template <typename T, typename U>
    requires std::derived_from<T, U>
    NonNull<RefPtr<T>> downcast(const NonNull<RefPtr<U>>& refPtr)
    {
        return NonNull<RefPtr<T>>(static_cast<T*>(refPtr.get().get()));
    }
}

using Foundation::NonNull;
