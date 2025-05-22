#pragma once

#include "window_button.h"

class WindowCloseButton final : public WindowButton {
public:
    explicit WindowCloseButton(const gfx::Rect& frame = {}) : WindowButton { frame } {}

    auto draw(cairo_t* cr) -> void override;
};
