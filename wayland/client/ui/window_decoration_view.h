#pragma once

#include <string>

#include "view.h"
#include "window.h"

namespace ui {

class TitleBar;
class WindowCloseButton;
class WindowMaximizeButton;

class Window::DecorationView : public View {
public:
    DecorationView(const std::string& text, const gfx::Rect& frame);

    virtual auto layout() -> void;

    auto draw(cairo_t* cr) -> void override;

private:
    std::shared_ptr<TitleBar> m_title_bar {};
};

}
