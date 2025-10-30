#pragma once

#include <Core/RefPtr.h>
#include <gfx/point.h>

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

        event(type type, const gfx::point& position, const RefPtr<gui::window>& window) :
            type { type },
            position { position },
            window { window }
        {
        }

        type type;
        gfx::point position;
        RefPtr<gui::window> window;
    };
}
