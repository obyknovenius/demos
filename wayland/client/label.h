#pragma once

#include "view.h"

#include <string>

class Label final : public View {
public:
        Label() : View {}
    {
    }

    explicit Label(const gfx::Rect& frame) : View { frame }
    {
    }

    auto text() -> std::string { return m_text; }
    auto set_text(const std::string& text) -> void { m_text = text; }

    auto intrinsic_size() -> std::optional<gfx::Size> override;

    auto draw(cairo_t*) -> void override;

private:
    std::string m_text {};

    std::string m_font_face { "ChicagoFLF" };
    double m_font_size { 24.0 };
};
