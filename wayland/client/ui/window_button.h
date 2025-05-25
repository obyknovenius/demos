#pragma once

#include "view.h"

namespace ui {

class WindowButton : public View {
public:
    explicit WindowButton(const gfx::Rect& frame = {}) : View { frame } {}

    auto draw(cairo_t* cr) -> void override;
};

}
