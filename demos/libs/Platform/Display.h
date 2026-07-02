#pragma once

#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>

namespace Platform
{
    class Display : public Object
    {
    public:
        static StrongPtr<Display> defaultDisplay()
        {
            return _defaultDisplay;
        }
    
    protected:
        Display() = default;
        ~Display() override = default;

    private:
        static StrongPtr<Display> _defaultDisplay;
    };
}
