#pragma once

#include <core/ref_ptr.h>

namespace gui {

class window;

struct event
{
    enum class type
    {
        invalid,
        button_pressed,
        button_released,
    };

    type type;
    ref_ptr<class window> window;
};

}
