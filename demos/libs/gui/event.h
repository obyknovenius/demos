#pragma once

#include <core/ref_ptr.h>

namespace gui {

class window;

struct event
{
    enum class type
    {
        button_pressed,
        button_released,
    };

    event(type type, const ref_ptr<class window>& window) : type { type }, window { window } {}

    type type;
    ref_ptr<class window> window;
};

}
