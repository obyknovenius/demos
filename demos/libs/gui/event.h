#pragma once

#include <Core/RefPtr.h>
#include <Gfx/Point.h>

namespace gui
{
    class window;

    struct event
    {
        enum class type
        {
            button_pressed,
            button_released,
        };

        event(type type, const Gfx::Point& position, const RefPtr<gui::window>& window) :
            type { type },
            position { position },
            window { window }
        {
        }

        type type;
        Gfx::Point position;
        RefPtr<gui::window> window;
    };
}
