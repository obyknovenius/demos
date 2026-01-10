#include "WindowMaximizeButton.h"

namespace GUI
{
    void Window::MaximizeButton::redraw(NonNull<RefPtr<Gfx::Context>> context)
    {
        context->fillRect(_bounds, Gfx::Color::White);

        int lineWidth = 2;
        context->strokeRect(_bounds.inset(lineWidth / 2), Gfx::Color::Black, 2.0f);

        auto innerRect = Gfx::Rect {
            origin: { 0, 0 },
            size: { _bounds.size.width / 2, _bounds.size.height / 2 }
        };
        context->strokeRect(innerRect.inset(lineWidth / 2), Gfx::Color::Black, 2.0f);
    }

    void Window::MaximizeButton::onPointerButtonPressed(Event event)
    {
        _pressed = true;
    }

    void Window::MaximizeButton::onPointerButtonReleased(Event event)
    {
        if (_pressed) {
            if (auto window = _window.strong())
                window->setMaximized(!window->maximized());
        }
        _pressed = false;
    }
}
