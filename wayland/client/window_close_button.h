#pragma once

#include "window_button.h"

class WindowCloseButton final : public WindowButton {
public:
    WindowCloseButton() : WindowButton {}
    {
    }

    explicit WindowCloseButton(const gfx::Rect& frame) : WindowButton { frame }
    {
    }

    auto draw(cairo_t*) -> void override;
};
