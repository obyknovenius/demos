#pragma once

#include <memory>
#include <string>

#include "view.h"

class TextView;
class WindowCloseButton;
class WindowMaximizeButton;

class TitleBar : public View {
public:
    TitleBar(const std::string& text, const gfx::Rect& frame);

    virtual auto layout() -> void;

    auto draw(cairo_t* cr) -> void override;

private:
    auto draw_placeholder(cairo_t* cr, const gfx::Rect& rect, float dx) -> void;

    std::shared_ptr<TextView> m_title_view {};
    std::shared_ptr<WindowCloseButton> m_close_button {};
    std::shared_ptr<WindowMaximizeButton> m_maximize_button {};
};
