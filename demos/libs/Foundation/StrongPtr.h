#pragma once

#include <concepts>
#include <utility>

namespace Foundation
{
    class Object;

    template <typename T>
    class WeakPtr;

    template <typename T>
    class StrongPtr
    {
        template <typename U>
        friend class StrongPtr;

        template <typename U>
        friend class WeakPtr;

    public:
        template <typename U>
        requires std::derived_from<U, Object> && std::convertible_to<U*, T*>
        static StrongPtr adopt(U* ptr)
        {
            return StrongPtr(ptr, Adopt{});
        }

        StrongPtr() = default;

        StrongPtr(std::nullptr_t) {}

        template <typename U>
        requires std::derived_from<U, Object> && std::convertible_to<U*, T*>
        StrongPtr(U* ptr) : _ptr(ptr), _object(ptr)
        {
            if (_object)
                _object->retain();
        }

        StrongPtr(const WeakPtr<T>& weakPtr)
        {
            if (Object* object = weakPtr._weakLink ? weakPtr._weakLink->get() : nullptr)
            {
                _ptr = weakPtr._ptr;
                _object = object;
                _object->retain();
            }
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        StrongPtr(const WeakPtr<U>& weakPtr)
        {
            if (Object* object = weakPtr._weakLink ? weakPtr._weakLink->get() : nullptr)
            {
                _ptr = weakPtr._ptr;
                _object = object;
                _object->retain();
            }
        }

        StrongPtr(const StrongPtr& other) : _ptr(other._ptr), _object(other._object)
        {
            if (_object)
                _object->retain();
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        StrongPtr(const StrongPtr<U>& other) : _ptr(other._ptr), _object(other._object)
        {
            if (_object)
                _object->retain();
        }

        StrongPtr(StrongPtr&& other) noexcept :
            _ptr(std::exchange(other._ptr, nullptr)),
            _object(std::exchange(other._object, nullptr))
        {
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        StrongPtr(StrongPtr<U>&& other) noexcept :
            _ptr(std::exchange(other._ptr, nullptr)),
            _object(std::exchange(other._object, nullptr))
        {
        }

        ~StrongPtr()
        {
            if (_object)
                _object->release();
        }

        T* get() const { return _ptr; }

        T* operator->() const { return get(); }
        T& operator*() const { return *get(); }

        explicit operator bool() const { return get(); }

        StrongPtr& operator=(std::nullptr_t) noexcept
        {
            StrongPtr tmp;
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::derived_from<U, Object> && std::convertible_to<U*, T*>
        StrongPtr& operator=(U* ptr) noexcept
        {
            StrongPtr tmp(ptr);
            swap(tmp);
            return *this;
        }

        StrongPtr& operator=(const StrongPtr& other) noexcept
        {
            StrongPtr tmp(other);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        StrongPtr& operator=(const StrongPtr<U>& other) noexcept
        {
            StrongPtr tmp(other);
            swap(tmp);
            return *this;
        }

        StrongPtr& operator=(StrongPtr&& other) noexcept
        {
            StrongPtr tmp(std::move(other));
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        StrongPtr& operator=(const WeakPtr<U>& weakPtr) noexcept
        {
            StrongPtr tmp(weakPtr);
            swap(tmp);
            return *this;
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        StrongPtr& operator=(StrongPtr<U>&& other) noexcept
        {
            StrongPtr tmp(std::move(other));
            swap(tmp);
            return *this;
        }

        void swap(StrongPtr<T>& other) noexcept
        {
            std::swap(_ptr, other._ptr);
            std::swap(_object, other._object);
        }

    private:
        enum class Adopt;

        template <typename U>
        requires std::derived_from<U, Object> && std::convertible_to<U*, T*>
        StrongPtr(U* ptr, Adopt) : _ptr(ptr), _object(ptr)
        {
        }

        T* _ptr = nullptr;
        Object* _object = nullptr;
    };

    template <typename T, typename... Args>
    requires std::derived_from<T, Object>
    StrongPtr<T> makeStrong(Args&&... args)
    {
        return StrongPtr<T>::adopt(new T(std::forward<Args>(args)...));
    }

    template <typename To, typename From>
    requires std::derived_from<To, From>
    StrongPtr<To> dynamicDowncast(const StrongPtr<From>& from)
    {
        return StrongPtr<To>(dynamic_cast<To*>(from.get()));
    }
}

using Foundation::StrongPtr;
using Foundation::makeStrong;
using Foundation::dynamicDowncast;
