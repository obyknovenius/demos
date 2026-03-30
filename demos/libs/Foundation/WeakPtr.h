#pragma once

#include "EnableWeakPtr.h"

namespace Foundation
{
    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() = default;

        WeakPtr(EnableWeakPtr<T>* ptr) : _weakLink { ptr ? ptr->weakLink() : nullptr } {}

        WeakPtr(const NonNullRefPtr<T>& ptr) : _weakLink { static_cast<EnableWeakPtr<T>*>(ptr.get())->weakLink() } {}

        template<typename U>
        requires(std::derived_from<U, EnableWeakPtr<T>>)
        WeakPtr(const NonNullRefPtr<U>& ptr) : _weakLink { static_cast<EnableWeakPtr<T>*>(ptr.get())->weakLink() } {}

        WeakPtr(const RefPtr<T>& ptr) : _weakLink { ptr ? static_cast<EnableWeakPtr<T>*>(ptr.get())->weakLink() : nullptr } {}

        template<typename U>
        requires(std::derived_from<U, EnableWeakPtr<T>>)
        WeakPtr(const RefPtr<U>& ptr) : _weakLink { ptr ? static_cast<EnableWeakPtr<T>*>(ptr.get())->weakLink() : nullptr } {}

        WeakPtr(const WeakPtr& other) : _weakLink { other._weakLink } {}

        WeakPtr(WeakPtr&& other) : _weakLink { std::move(other._weakLink) } {}

        WeakPtr& operator=(std::nullptr_t)
        {
            WeakPtr tmp;
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(EnableWeakPtr<T>* ptr)
        {
            WeakPtr tmp { ptr };
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(const NonNullRefPtr<T>& ptr)
        {
            WeakPtr tmp { ptr };
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(const RefPtr<T>& ptr)
        {
            WeakPtr tmp { ptr };
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(const WeakPtr& other)
        {
            WeakPtr tmp (other);
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(WeakPtr&& other)
        {
            WeakPtr tmp (std::move(other));
            swap(tmp);
            return *this;
        }

        T* get() const
        {
            if (_weakLink)
                return _weakLink->get();
            return nullptr;
        }

        T* operator->() const { return get(); }
        T& operator*() const { return *get(); }

        bool operator==(std::nullptr_t) const { return !get(); }
        bool operator==(const WeakPtr& other) const { return get() == other.get(); }

        explicit operator bool() const { return get(); }

        void swap(WeakPtr& other)
        {
            std::swap(_weakLink, other._weakLink);
        }

    private:
        RefPtr<WeakLink<T>> _weakLink;
    };
}

using Foundation::WeakPtr;
