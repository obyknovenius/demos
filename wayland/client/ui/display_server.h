#pragma once

#include "gfx/rect.h"

#include <memory>
#include <mutex>

namespace ui {

class DisplayServer {
public:
    static auto shared() -> std::shared_ptr<DisplayServer>;

    virtual ~DisplayServer() = default;

    virtual auto create_window(gfx::Rect frame) -> int = 0;

    virtual auto run() -> void = 0;

protected:
    static DisplayServer* create();
};

}
