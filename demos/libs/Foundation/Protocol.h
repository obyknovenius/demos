#pragma once

#include "RefCounted.h"
#include "EnableWeakPtr.h"

namespace Foundation
{
    class Protocol : virtual public RefCounted, virtual public EnableWeakPtr
    {
    };
}

using Foundation::Protocol;
