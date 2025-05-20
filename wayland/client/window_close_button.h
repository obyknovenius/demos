#pragma once

#include "window_button.h"

class WindowCloseButton final : public WindowButton {
public:
    WindowCloseButton(float x, float y) : WindowButton {x, y} {}

    auto draw(cairo_t*) -> void override;
};
