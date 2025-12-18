#pragma once

#include <Core/OptionSet.h>
#include <Core/RefPtr.h>
#include <Gfx/Point.h>
#include <optional>

namespace GUI
{
    class Window;

    struct Event
    {
        enum class Type
        {
            pointerButtonPressed,
            pointerButtonReleased,
            pointerEntered,
            pointerMoved,
            pointerLeft,
        };

        enum class PointerButton
        {
            mouseLeft = 1 << 0,
            mouseRight = 1 << 1,
            mouseMiddle = 1 << 2,
        };

        using PointerButtons = OptionSet<PointerButton>;

        Type type;
        RefPtr<Window> window;
        std::optional<Gfx::Point> position;
        PointerButtons pressedPointerButtons;
    };
}
