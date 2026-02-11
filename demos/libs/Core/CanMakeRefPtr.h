#pragma once

namespace Core
{
    class CanMakeRefPtr
    {
    public:
        virtual void ref() = 0;
        virtual void unref() = 0;
    protected:
        virtual ~CanMakeRefPtr() = default;
    };
}

using Core::CanMakeRefPtr;
