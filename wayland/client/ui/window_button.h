#pragma once

#include "view.h"

class WindowButton : public View {
public:
    WindowButton() : View {}
    {
    }

    explicit WindowButton(const gfx::Rect& frame) : View { frame }
    {
    }

    auto draw(cairo_t*) -> void override;
};
