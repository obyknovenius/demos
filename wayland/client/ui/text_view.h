#pragma once

#include "view.h"

#include <string>

namespace ui {

class TextView final : public View {
public:
    explicit TextView(const gfx::Rect& frame) : View { frame } {}

    TextView(const std::string& text, const gfx::Rect& frame = {})
        : View { frame }
        , m_text { text }
    {
    }

    auto text() -> std::string { return m_text; }
    auto set_text(const std::string& text) -> void { m_text = text; }

    auto intrinsic_size() -> std::optional<gfx::Size> override;

    auto draw(cairo_t* cr) -> void override;

private:
    std::string m_text {};

    std::string m_font_face { "ChicagoFLF" };
    double m_font_size { 24.0 };
};

}
