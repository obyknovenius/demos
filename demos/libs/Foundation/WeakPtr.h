#pragma once

#include "EnableWeakPtr.h"

namespace Foundation
{
    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() = default;

        WeakPtr(std::nullptr_t) : _weakLink { nullptr } {}

        WeakPtr(EnableWeakPtr<T>* ptr) : _weakLink { ptr ? ptr->weakLink() : nullptr } {}

        WeakPtr(const WeakPtr& other) : _weakLink { other._weakLink } {}

        WeakPtr(WeakPtr&& other) : _weakLink { std::move(other._weakLink) } {}

        WeakPtr& operator=(std::nullptr_t)
        {
            _weakLink = nullptr;
            return *this;
        }

        WeakPtr& operator=(EnableWeakPtr<T>* ptr)
        {
            _weakLink = ptr ? ptr->weakLink() : nullptr;
            return *this;
        }

        WeakPtr& operator=(const WeakPtr& other)
        {
            _weakLink = other._weakLink;
            return *this;
        }

        WeakPtr& operator=(WeakPtr&& other)
        {
            _weakLink = std::move(other._weakLink);
            return *this;
        }

        T* get() const
        {
            if (_weakLink)
                return static_cast<T*>(_weakLink->get());
            return nullptr;
        }

        T* operator->() const { return get(); }
        T& operator*() const { return *get(); }

        bool operator==(std::nullptr_t) const { return get() == nullptr; }
        bool operator==(const WeakPtr& other) const { return get() == other.get(); }

        explicit operator bool() const { return get() != nullptr; }

        void reset()
        {
            _weakLink = nullptr;
        }

    private:
        RefPtr<WeakLink<T>> _weakLink;
    };
}

using Foundation::WeakPtr;
