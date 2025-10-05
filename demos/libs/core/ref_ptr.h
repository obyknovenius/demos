#pragma once

#include <core/nonnull_ref_ptr.h>
#include <utility>

namespace core
{
    template <typename T>
    class ref_ptr
    {
    public:
        enum adopt_tag { adopt };

        ref_ptr(T* ptr = nullptr) : m_ptr(ptr)
        {
            if (m_ptr)
                m_ptr->ref();
        }

        ref_ptr(adopt_tag, T* ptr) : m_ptr(ptr)
        {
        }

        ref_ptr(const ref_ptr& other) : m_ptr(other.m_ptr)
        {
            if (m_ptr)
                m_ptr->ref();
        }

        template<typename U>
        ref_ptr(const ref_ptr<U>& other) : m_ptr(static_cast<T*>(other.ptr()))
        {
            if (m_ptr)
                m_ptr->ref();
        }

        ref_ptr(const nonnull_ref_ptr<T>& other) : m_ptr(other.ptr())
        {
            m_ptr->ref();
        }

        template<typename U>
        ref_ptr(const nonnull_ref_ptr<U>& other) : m_ptr(static_cast<T*>(other.ptr()))
        {
            m_ptr->ref();
        }

        ~ref_ptr()
        {
            if (m_ptr)
                m_ptr->unref();
        }

        ref_ptr& operator=(const ref_ptr& other)
        {
            if (this != &other)
            {
                if (m_ptr)
                    m_ptr->unref();
                m_ptr = other.m_ptr;
                if (m_ptr)
                    m_ptr->ref();
            }
            return *this;
        }

        T* ptr() const { return m_ptr; }
        T& operator*() const { return *m_ptr; }
        T* operator->() const { return m_ptr; }

        operator bool() const { return m_ptr; }

    private:
        T* m_ptr { nullptr };
    };

    template<typename T>
    ref_ptr<T> adopt(T* ptr)
    {
        return ref_ptr<T>(ref_ptr<T>::adopt, ptr);
    }
}

using core::ref_ptr;
using core::adopt;
