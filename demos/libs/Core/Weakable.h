#pragma once

#include "NonnullRefPtr.h"
#include "RefCounted.h"
#include "RefPtr.h"

namespace Core
{
    class Weakable;

    class WeakLink : public RefCounted
    {
    public:
        WeakLink(Weakable& ref) : _ptr { &ref }
        {
        }

        ~WeakLink() = default;

        RefPtr<Weakable> WeakLink::strong()
        {
            return _ptr;
        }

    private:
        Weakable* _ptr { nullptr };
    };

    class Weakable : public RefCounted
    {
        template<typename T>
        friend class WeakPtr;

    public:
        Weakable::Weakable() : _weakLink { adopt(*new WeakLink(*this)) }
        {
        }

        ~Weakable() = default;

    private:
        NonnullRefPtr<WeakLink> _weakLink;
    };
}

using Core::Weakable;
