#pragma once

#include "Forward.h"
#include <Core/OptionSet.h>
#include <Core/RefPtr.h>
#include <Gfx/Point.h>
#include <optional>

namespace GUI
{
    struct Event
    {
        enum class Type
        {
            PointerButtonPressed,
            PointerButtonReleased,
            PointerEntered,
            PointerMoved,
            PointerLeft,
        };

        enum class PointerButton
        {
            MouseLeft = 1 << 0,
            MouseRight = 1 << 1,
            MouseMiddle = 1 << 2,
        };

        using PointerButtons = OptionSet<PointerButton>;

        Type type;
        RefPtr<Window> window;
        std::optional<Gfx::Point> position;
        PointerButtons pressedPointerButtons;
    };
}
