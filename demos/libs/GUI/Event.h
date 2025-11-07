#pragma once

#include <Core/RefPtr.h>
#include <Gfx/Point.h>

namespace GUI
{
    class Window;

    struct Event
    {
        enum class Type
        {
            ButtonPressed,
            ButtonReleased,
        };

        Event(Type type, const Gfx::Point& position, const RefPtr<GUI::Window>& window) :
            type { type },
            position { position },
            window { window }
        {
        }

        Type type;
        Gfx::Point position;
        RefPtr<GUI::Window> window;
    };
}
