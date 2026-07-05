#pragma once

#include <Foundation/NonNull.h>
#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>

namespace Platform
{
    class Display : public Object
    {
    public:
        static NonNull<StrongPtr<Display>> defaultDisplay()
        {
            return _defaultDisplay;
        }

    protected:
        Display() = default;
        ~Display() override = default;

    private:
        static NonNull<StrongPtr<Display>> _defaultDisplay;
    };
}
