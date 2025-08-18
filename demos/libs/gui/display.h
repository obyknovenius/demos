#pragma once

#include <memory>
#include "window.h"

namespace gui {

class display
{
public:
    static auto connect() -> std::shared_ptr<display>;

    virtual ~display() = default;

    virtual auto create_window() -> std::shared_ptr<window> = 0;
};

}
