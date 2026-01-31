#pragma once

#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/Weakable.h>

namespace Platform
{
    class Display : public RefCounted, public Weakable
    {
    public:
        static NonNull<RefPtr<Display>> defaultDisplay() { return _defaultDisplay; }

    private:
        static RefPtr<Display> _defaultDisplay;
    };
}
