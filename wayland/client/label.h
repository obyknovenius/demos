#pragma once

#include "view.h"

#include <string>

class Label final : public View {
public:
    Label(float x, float y, float width, float height)
        : View { x, y, width, height }
    {}

    auto text() -> std::string { return m_text; }
    auto set_text(const std::string& text) -> void { m_text = text; }

    auto layout() -> void;

    auto draw(cairo_t*) -> void override;

private:
    std::string m_text {};

    std::string m_font_face { "ChicagoFLF" };
    double m_font_size { 24.0 };
};
