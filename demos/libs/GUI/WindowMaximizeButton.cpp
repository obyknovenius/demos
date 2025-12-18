#include "WindowMaximizeButton.h"

namespace GUI
{
    void Window::MaximizeButton::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect(_bounds, Gfx::Color::white);

        int lineWidth = 2;
        context->strokeRect(_bounds.inset(lineWidth / 2), Gfx::Color::black, 2.0f);

        auto innerRect = Gfx::Rect {
            origin: { 0, 0 },
            size: { _bounds.size.width / 2, _bounds.size.height / 2 }
        };
        context->strokeRect(innerRect.inset(lineWidth / 2), Gfx::Color::black, 2.0f);
    }

    void Window::MaximizeButton::onPointerButtonPressed(const Event& event)
    {
        _pressed = true;
    }

    void Window::MaximizeButton::onPointerButtonReleased(const Event& event)
    {
        if (_pressed) {
            if (auto window = _window.strong())
                window->setMaximized(!window->maximized());
        }
        _pressed = false;
    }
}
