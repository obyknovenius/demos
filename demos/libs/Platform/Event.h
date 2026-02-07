#pragma once

namespace Platform
{
    struct Event
    {
        enum class Type
        {
            PointerEntered,
            PointerMoved,
            PointerLeft
        };

        Type type;
    };
}
