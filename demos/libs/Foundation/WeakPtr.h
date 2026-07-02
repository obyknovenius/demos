#pragma once

#include <concepts>
#include <utility>

namespace Foundation
{
    template <typename T>
    class WeakPtr
    {
        template <typename U>
        friend class StrongPtr;

    public:
        WeakPtr() = default;

        WeakPtr(std::nullptr_t) {}

        template <typename U>
        requires std::convertible_to<U*, T*>
        WeakPtr(const StrongPtr<U>& strongPtr)
        {
            if (strongPtr._object)
            {
                _ptr = strongPtr._ptr;
                _weakLink = strongPtr._object->weakLink();
                _weakLink->retain();
            }
        }

        WeakPtr(const WeakPtr& other) : _ptr(other._ptr), _weakLink(other._weakLink)
        {
            if (_weakLink)
                _weakLink->retain();
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        WeakPtr(const WeakPtr<U>& other) : _ptr(other._ptr), _weakLink(other._weakLink)
        {
            if (_weakLink)
                _weakLink->retain();
        }

        WeakPtr(WeakPtr&& other) noexcept :
            _ptr(std::exchange(other._ptr, nullptr)),
            _weakLink(std::exchange(other._weakLink, nullptr)) {}

        template <typename U>
        requires std::convertible_to<U*, T*>
        WeakPtr(WeakPtr<U>&& other) noexcept :
            _ptr(std::exchange(other._ptr, nullptr)),
            _weakLink(std::exchange(other._weakLink, nullptr)) {}

        ~WeakPtr()
        {
            if (_weakLink)
                _weakLink->release();
        }

        T* get() const { return (_weakLink && _weakLink->get()) ? _ptr : nullptr; }

        WeakPtr& operator=(std::nullptr_t) noexcept
        {
            WeakPtr tmp;
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(const WeakPtr& other) noexcept
        {
            WeakPtr tmp(other);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        WeakPtr& operator=(const WeakPtr<U>& other) noexcept
        {
            WeakPtr tmp(other);
            swap(tmp);
            return *this;
        }

        WeakPtr& operator=(WeakPtr&& other) noexcept
        {
            WeakPtr tmp(std::move(other));
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        WeakPtr& operator=(WeakPtr<U>&& other) noexcept
        {
            WeakPtr tmp(std::move(other));
            swap(tmp);
            return *this;
        }

        void swap(WeakPtr& other) noexcept
        {
            std::swap(_ptr, other._ptr);
            std::swap(_weakLink, other._weakLink);
        }

    private:
        T* _ptr = nullptr;
        WeakLink* _weakLink = nullptr;
    };
}

using Foundation::WeakPtr;
