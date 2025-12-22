#include "WindowCloseButton.h"

namespace GUI
{
    void Window::CloseButton::redraw(RefPtr<Gfx::Context> context)
    {
        context->fillRect(_bounds, Gfx::Color::white);

        int lineWidth = 2;
        context->strokeRect(_bounds.inset(lineWidth / 2), Gfx::Color::black, 2.0f);

        auto crossPadding = 5;
        auto crossRect = _bounds.inset(crossPadding);

        context->strokeLine({ crossRect.minX(), crossRect.minY() }, { crossRect.maxX(), crossRect.maxY() }, Gfx::Color::black, 2.0f);
        context->strokeLine({ crossRect.maxX(), crossRect.minY() }, { crossRect.minX(), crossRect.maxY() }, Gfx::Color::black, 2.0f);
    }

    void Window::CloseButton::onPointerButtonPressed(const Event& event)
    {
        _pressed = true;
    }

    void Window::CloseButton::onPointerButtonReleased(const Event& event)
    {
        if (_pressed) {
            if (auto window = _window.strong())
                window->close();
        }
        _pressed = false;
    }
}
