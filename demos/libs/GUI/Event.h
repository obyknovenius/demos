#pragma once

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
            PointerButtonPressed,
            PointerButtonReleased,
            PointerEntered,
            PointerMoved,
            PointerLeft,
        };

        Event(Type type, const std::optional<Gfx::Point>& position, const RefPtr<GUI::Window>& window) :
            type { type },
            position { position },
            window { window }
        {
        }

        Type type;
        std::optional<Gfx::Point> position;
        RefPtr<GUI::Window> window;
    };
}
