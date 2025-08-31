#pragma once

#include "event.h"
#include <functional>

namespace gui {

class window
{
public:
    window() = default;
    virtual ~window() = default;

    auto dispatch_event(const event event) -> void;

    virtual auto close() -> void;

    std::function<void()> on_close;
};

}
