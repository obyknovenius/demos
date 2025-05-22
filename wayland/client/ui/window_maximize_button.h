#pragma once

#include "window_button.h"

class WindowMaximizeButton final : public WindowButton {
public:
    WindowMaximizeButton() : WindowButton {}
    {
    }

    explicit WindowMaximizeButton(const gfx::Rect& frame) : WindowButton { frame }
    {
    }

    auto draw(cairo_t*) -> void override;
};
