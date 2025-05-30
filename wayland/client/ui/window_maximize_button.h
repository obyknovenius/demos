#pragma once

#include "window_button.h"

namespace ui {

class WindowMaximizeButton final : public WindowButton {
public:
    explicit WindowMaximizeButton(const gfx::Rect& frame = {}) : WindowButton { frame } {}

    auto draw(cairo_t* cr) -> void override;
};

}
