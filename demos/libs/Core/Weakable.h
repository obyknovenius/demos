#pragma once

#include "NonNull.h"
#include "RefCounted.h"
#include "RefPtr.h"

namespace Core
{
    class Weakable;

    class WeakLink final : public RefCounted
    {
        friend class Weakable;

    public:
        template<typename T>
        RefPtr<T> strong() const { return static_cast<T*>(_ptr); }

        void revoke() { _ptr = nullptr; }

    private:
        explicit WeakLink(NonNull<Weakable*> ptr) :
            _ptr { ptr }
        {
        }

        Weakable* _ptr {};
    };

    class Weakable
    {
        template<typename T>
        friend class WeakPtr;

    public:
        Weakable() :
            _link { adopt(new WeakLink(this)) }
        {
        }

        virtual ~Weakable()
        {
            _link->revoke();
        }

    private:
        RefPtr<WeakLink> _link;
    };
}

using Core::Weakable;
