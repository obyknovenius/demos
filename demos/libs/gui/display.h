#pragma once

#include <memory>
#include "event.h"

namespace gui {

class window;

class display
{
public:
    static auto connect() -> std::shared_ptr<display>;

    virtual ~display() = default;

    virtual auto create_window() -> std::shared_ptr<window> = 0;

    auto dispatch_event(const event event) -> void;
};

}
