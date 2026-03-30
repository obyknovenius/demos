#pragma once

#include <Foundation/EnableWeakPtr.h>
#include <Foundation/NonNull.h>
#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>

namespace Platform
{
    class Display : public RefCounted, public EnableWeakPtr<Display>
    {
    public:
        static NonNull<RefPtr<Display>> defaultDisplay() { return _defaultDisplay; }

    private:
        static RefPtr<Display> _defaultDisplay;
    };
}
