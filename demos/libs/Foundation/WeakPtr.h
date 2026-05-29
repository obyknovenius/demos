#pragma once

#include "EnableWeakPtr.h"
#include "NonNull.h"

namespace Foundation
{
    template <typename T>
    class WeakPtr
    {
        template <typename U>
        friend class WeakPtr;

    public:
        WeakPtr() = default;

        WeakPtr(T* ptr) : _weakLink(ptr ? ptr->weakLink() : nullptr) {}

        WeakPtr(const RefPtr<T>& refPtr) : WeakPtr(refPtr.get()) {}

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr(const RefPtr<U>& refPtr) : WeakPtr(refPtr.get()) {}

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr(const NonNull<RefPtr<U>>& refPtr) : WeakPtr(refPtr.get()) {}

        WeakPtr(const WeakPtr<T>& other) : _weakLink(other._weakLink) {}

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr(const WeakPtr<U>& other) : _weakLink(other._weakLink) {}

        WeakPtr(WeakPtr&& other) : _weakLink(std::move(other._weakLink)) {}

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr(WeakPtr<U>&& other) : _weakLink(std::move(other._weakLink)) {}

        T* get() const { return _weakLink ? dynamic_cast<T*>(_weakLink->get()) : nullptr; }

        T* operator->() const { return get(); }
        std::remove_pointer_t<T>& operator*() const { return *get(); }

        explicit operator bool() const { return get(); }

        WeakPtr& operator=(T* ptr)
        {
            WeakPtr tmp(ptr);
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(const RefPtr<T>& refPtr)
        {
            WeakPtr tmp(refPtr);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr& operator=(const RefPtr<U>& refPtr)
        {
            WeakPtr tmp(refPtr);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr& operator=(const NonNull<RefPtr<U>>& refPtr)
        {
            WeakPtr tmp(refPtr);
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(const WeakPtr& other)
        {
            WeakPtr tmp(other);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr& operator=(const WeakPtr<U>& other)
        {
            WeakPtr tmp(other);
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(WeakPtr&& other)
        {
            WeakPtr tmp(std::move(other));
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, T>
        WeakPtr& operator=(WeakPtr<U>&& other)
        {
            WeakPtr tmp(std::move(other));
            swap(tmp);
            return *this;
        }

        void swap(WeakPtr& other)
        {
            _weakLink.swap(other._weakLink);
        }

    private:
        RefPtr<WeakLink> _weakLink;
    };
}

using Foundation::WeakPtr;
