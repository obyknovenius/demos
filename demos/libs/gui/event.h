#pragma once

#include <core/ref_ptr.h>
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

        event(type type, const gfx::point& position, const ref_ptr<gui::window>& window) :
            type { type },
            position { position },
            window { window }
        {
        }

        type type;
        gfx::point position;
        ref_ptr<gui::window> window;
    };
}
