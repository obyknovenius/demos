#pragma once

#include "nonnull_ref_ptr.h"
#include "ref_ptr.h"
#include "weakable.h"

namespace core
{
    template<typename T>
    class weak_ptr
    {
    public:
        weak_ptr() = default;

        weak_ptr(const T* ptr)
        {
            if (ptr)
                m_link = ptr->m_weak_link;
        }

        weak_ptr(const ref_ptr<T>& ptr)
        {
            if (ptr)
                m_link = ptr->m_weak_link;
        }

        weak_ptr(const nonnull_ref_ptr<T>& ptr) : m_link { ptr->m_weak_link } {}

        ref_ptr<T> strong_ref()
        {
            if (m_link)
                return m_link->strong_ref();
            return nullptr;
        }
    private:
        ref_ptr<weak_link> m_link {};
    };
}

using core::weak_ptr;
