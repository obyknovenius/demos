#pragma once

#include "window_button.h"

class WindowMaximizeButton final : public WindowButton {
public:
    WindowMaximizeButton(float x, float y) : WindowButton {x, y} {}

    auto draw(cairo_t*) -> void override;
};
