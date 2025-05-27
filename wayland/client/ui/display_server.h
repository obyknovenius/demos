#pragma once

#include "gfx/size.h"

#include <memory>

namespace ui {

class DisplayServer {
public:
    static auto shared() -> std::shared_ptr<DisplayServer>;

    virtual ~DisplayServer() = default;

    virtual auto create_window(gfx::Size size) -> int = 0;

    virtual auto run() -> void = 0;

protected:
    static DisplayServer* create();
};

}
