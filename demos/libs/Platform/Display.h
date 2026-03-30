#pragma once

#include <Foundation/EnableWeakPtr.h>
#include <Foundation/NonNullRefPtr.h>
#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>

namespace Platform
{
    class Display : public RefCounted, public EnableWeakPtr<Display>
    {
    public:
        static NonNullRefPtr<Display> defaultDisplay() { return _defaultDisplay; }

    private:
        static RefPtr<Display> _defaultDisplay;
    };
}
