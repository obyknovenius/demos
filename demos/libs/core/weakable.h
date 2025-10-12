#pragma once

#include "ref_counted.h"
#include "ref_ptr.h"

namespace core
{
    class weakable;

    class weak_link : public ref_counted
    {
    public:
        weak_link(weakable& ref) : m_ptr(&ref)
        {
        }

        ~weak_link() = default;

        ref_ptr<weakable> strong_ref()
        {
            return m_ptr;
        }

        void reset()
        {
            m_ptr = nullptr;
        }

    private:
        weakable* m_ptr { nullptr };

        int m_ref_count { 0 };
    };

    class weakable : public ref_counted
    {
        template<typename T>
        friend class weak_ptr;

    public:
        weakable() : m_weak_link { adopt(new weak_link(*this)) }
        {
        }

        ~weakable()
        {
            m_weak_link->reset();
        }

    private:
        ref_ptr<weak_link> m_weak_link {};
    };
}

using core::weakable;
