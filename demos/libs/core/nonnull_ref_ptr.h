#pragma once

#include <utility>

namespace core
{
    template <typename T>
    class nonnull_ref_ptr
    {
    public:
        enum adopt_tag { adopt };

        nonnull_ref_ptr(T& ref) : m_ptr(&ref)
        {
            if (m_ptr)
                m_ptr->ref();
        }

        nonnull_ref_ptr(adopt_tag, T& ref) : m_ptr(&ref)
        {
        }

        nonnull_ref_ptr(const nonnull_ref_ptr& other) : m_ptr(other.m_ptr)
        {
            if (m_ptr)
                m_ptr->ref();
        }

        template<typename U>
        nonnull_ref_ptr(const nonnull_ref_ptr<U>& other) : m_ptr(static_cast<T*>(other.ptr()))
        {
            m_ptr->ref();
        }

        ~nonnull_ref_ptr()
        {
            if (m_ptr)
                m_ptr->unref();
        }

        nonnull_ref_ptr& operator=(const nonnull_ref_ptr& other)
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
    auto adopt(T& ref) -> nonnull_ref_ptr<T>
    {
        return nonnull_ref_ptr<T>(nonnull_ref_ptr<T>::adopt, ref);
    }
}

using core::nonnull_ref_ptr;
using core::adopt;
