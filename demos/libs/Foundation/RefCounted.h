#pragma once

namespace Foundation
{
    class RefCounted
    {
    public:
        void ref()
        {
            ++_refCount;
        }

        void unref()
        {
            if (--_refCount == 0)
                delete this;
        }

        unsigned refCount() const { return _refCount; }

    protected:
        RefCounted() = default;
        virtual ~RefCounted() = default;

    private:
        unsigned _refCount { 1 };
    };
}

using Foundation::RefCounted;
