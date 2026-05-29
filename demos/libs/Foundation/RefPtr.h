#pragma once

#include "NonNull.h"
#include <concepts>
#include <utility>

namespace Foundation
{
    template <typename T>
    class WeakPtr;

    template <typename T>
    class RefPtr
    {
        template <typename U>
        friend class RefPtr;

    public:
        static RefPtr adopt(T* ptr) { return RefPtr(ptr, Adopt::Yes); }

        RefPtr() = default;

        RefPtr(T* ptr) : _ptr(ptr)
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(const NonNull<T*> nonNullPtr) : _ptr(nonNullPtr.get())
        {
            _ptr->ref();
        }

        RefPtr(const RefPtr<T>& refPtr) : _ptr(refPtr.get())
        {
            if (_ptr)
                _ptr->ref();
        }

        template <typename U>
        requires std::derived_from<U, T>
        RefPtr(const RefPtr<U>& refPtr) : _ptr(static_cast<T*>(refPtr.get()))
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(const NonNull<RefPtr<T>>& nonNullRefPtr) : _ptr(nonNullRefPtr.get().get())
        {
            _ptr->ref();
        }

        template <typename U>
        requires std::derived_from<U, T>
        RefPtr(const NonNull<RefPtr<U>>& nonNullRefPtr) : _ptr(static_cast<T*>(nonNullRefPtr.get().get()))
        {
            _ptr->ref();
        }

        RefPtr(const WeakPtr<T>& weakPtr) : _ptr(weakPtr.get())
        {
            if (_ptr)
                _ptr->ref();
        }

        template <typename U>
        requires std::derived_from<U, T>
        RefPtr(const WeakPtr<U>& weakPtr) : _ptr(static_cast<T*>(weakPtr.get()))
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(RefPtr<T>&& ptr) : _ptr(std::exchange(ptr._ptr, nullptr))
        {
        }

        template <typename U>
        requires std::derived_from<U, T>
        RefPtr(RefPtr<U>&& ptr) : _ptr(static_cast<T*>(std::exchange(ptr._ptr, nullptr)))
        {
        }

        ~RefPtr()
        {
            if (_ptr)
                _ptr->unref();
        }

        T* get() const { return _ptr; }

        T* operator->() const { return get(); }
        T& operator*() const { return *get(); }

        explicit operator bool() const { return get(); }

        RefPtr& operator=(T* ptr)
        {
            RefPtr tmp(ptr);
            swap(tmp);
            return *this;
        }

        RefPtr& operator=(const NonNull<T*> nonNullPtr)
        {
            RefPtr tmp(nonNullPtr);
            swap(tmp);
            return *this;
        }

        RefPtr& operator=(const RefPtr<T>& refPtr)
        {
            RefPtr tmp(refPtr);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, T>
        RefPtr& operator=(const RefPtr<U>& refPtr)
        {
            RefPtr tmp(refPtr);
            swap(tmp);
            return *this;
        }

        RefPtr& operator=(const NonNull<RefPtr<T>>& nonNullRefPtr)
        {
            RefPtr tmp(nonNullRefPtr);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, T>
        RefPtr& operator=(const NonNull<RefPtr<U>>& nonNullRefPtr)
        {
            RefPtr tmp(nonNullRefPtr);
            swap(tmp);
            return *this;
        }

        RefPtr& operator=(const WeakPtr<T>& weakPtr)
        {
            RefPtr tmp(weakPtr);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, T>
        RefPtr& operator=(const WeakPtr<U>& weakPtr)
        {
            RefPtr tmp(weakPtr);
            swap(tmp);
            return *this;
        }

        RefPtr& operator=(RefPtr&& refPtr)
        {
            RefPtr tmp(std::move(refPtr));
            swap(tmp);
            return *this;
        }

        void swap(RefPtr& refPtr)
        {
            std::swap(_ptr, refPtr._ptr);
        }

    private:
        enum class Adopt { Yes };

        RefPtr(T* ptr, Adopt) : _ptr(ptr) {}

        T* _ptr = nullptr;
    };

    template <typename T, typename U>
    requires std::derived_from<T, U>
    RefPtr<T> downcast(const RefPtr<U>& refPtr)
    {
        return RefPtr<T>(static_cast<T*>(refPtr.get()));
    }
}

using Foundation::RefPtr;
