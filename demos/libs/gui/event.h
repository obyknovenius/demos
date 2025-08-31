#pragma once

#include <memory>

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
    std::shared_ptr<class window> window;
};

}
