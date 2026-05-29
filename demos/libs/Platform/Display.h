#pragma once

#include <Foundation/NonNull.h>
#include <Foundation/Object.h>

namespace Platform
{
    class Display : public Object
    {
    public:
        static NonNull<RefPtr<Display>> defaultDisplay() { return _defaultDisplay; }

    private:
        static RefPtr<Display> _defaultDisplay;
    };
}
