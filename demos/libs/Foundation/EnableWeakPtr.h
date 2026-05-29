#pragma once

#include "RefCounted.h"
#include "RefPtr.h"

namespace Foundation
{
    class EnableWeakPtr;

    class WeakLink final : public RefCounted
    {
    public:
        WeakLink(EnableWeakPtr* ptr) : _ptr(ptr) {}
        ~WeakLink() = default;

        EnableWeakPtr* get() const { return _ptr; }

        void revoke() { _ptr = nullptr; }

    private:
        EnableWeakPtr* _ptr;
    };

    class EnableWeakPtr
    {
        template <typename T>
        friend class WeakPtr;

    public:
        EnableWeakPtr() = default;

        virtual ~EnableWeakPtr()
        {
            if (_weakLink)
                _weakLink->revoke();
        }

    private:
        RefPtr<WeakLink> weakLink()
        {
            if (!_weakLink)
                _weakLink = RefPtr<WeakLink>::adopt(new WeakLink(this));
            return _weakLink;
        }

        RefPtr<WeakLink> _weakLink;
    };
}

using Foundation::EnableWeakPtr;
