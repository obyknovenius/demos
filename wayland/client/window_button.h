#pragma once

#include "view.h"

class WindowButton : public View {
public:
    WindowButton(float x, float y) : View { x, y, m_size + 2 * m_margin, m_size } {}

    auto draw(cairo_t*) -> void override;

protected:
    static constexpr float m_size { 22.0f };
    static constexpr float m_margin { 2.0f };
};
