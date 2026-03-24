#pragma once

namespace Foundation
{
    template<typename T>
    class RefPtr
    {
    public:
        static RefPtr adopt(T* ptr) { return RefPtr(Adopt, ptr); }

        RefPtr() = default;

        RefPtr(T* ptr) : _ptr { ptr }
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
        requires(std::derived_from<U, T>)
        RefPtr(const RefPtr<U>& other) : _ptr { static_cast<T*>(other.get()) }
        {
            if (_ptr)
                _ptr->ref();
        }

        RefPtr(RefPtr&& other) : _ptr { other.leak() } {}

        template<typename U>
        requires(std::derived_from<U, T>)
        RefPtr(RefPtr<U>&& other) : _ptr { static_cast<T*>(other.leak()) } {}

        ~RefPtr()
        {
            if (_ptr)
                _ptr->unref();
        }

        RefPtr& operator=(T* ptr)
        {
            RefPtr tmp { ptr };
            swap(tmp);
            return *this;
        }

        RefPtr& operator=(const RefPtr& other)
        {
            RefPtr tmp { other };
            swap(tmp);
            return *this;
        }

        template<typename U>
        requires(std::derived_from<U, T>)
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
        requires(std::derived_from<U, T>)
        RefPtr& operator=(RefPtr<U>&& other)
        {
            RefPtr tmp { std::move(other) };
            swap(tmp);
            return *this;
        }

        T* get() const { return _ptr; }

        T* operator->() const { return get(); }
        T& operator*() const { return *get(); }

        bool operator==(std::nullptr_t) const { return !get(); }
        bool operator==(const RefPtr& other) const { return get() == other.get(); }

        explicit operator bool() const { return get(); }

        [[nodiscard]] T* leak()
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
}

using Foundation::RefPtr;
