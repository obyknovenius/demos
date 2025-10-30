#pragma once

namespace Core
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

    protected:
        RefCounted() = default;
        virtual ~RefCounted() = default;

    private:
        int _refCount { 1 };
    };
}

using Core::RefCounted;
