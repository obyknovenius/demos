#pragma once

#include "RefCounted.h"
#include "EnableWeakPtr.h"

namespace Foundation
{
    class Object : virtual public RefCounted, virtual public EnableWeakPtr
    {
    };
}

using Foundation::Object;
