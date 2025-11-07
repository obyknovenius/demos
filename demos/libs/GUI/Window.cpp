#include "Window.h"

#include "Event.h"
#include "WindowDecorationView.h"
#include "WindowTitleBar.h"
#include "Wayland/Window.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>
#include <Gfx/Rect.h>

namespace GUI
{
    NonnullRefPtr<Window> Window::make()
    {
        return Wayland::Window::make();
    }

    Window::Window(Gfx::Size const& size) :
        _size { size },
        _decorationView { DecorationView::make(*this) }
    {
    }

    Window::~Window() = default;

    bool Window::shouldStartMove(const Gfx::Point& pointer_position) const
    {
        if (_decorationView->titleBar()->frame().contains(pointer_position))
            return true;
        return false;
    }

    void Window::dispatchEvent(std::unique_ptr<const Event> event)
    {
        switch (event->type)
        {
            case Event::Type::ButtonPressed:
            {
                auto view = _decorationView->hitTest(event->position);
                view->onButtonPressed(std::move(event));
                break;
            }

            default:
                break;
        }
    }

    void Window::close()
    {
        if (onClose)
            onClose();
    }

    void Window::layout()
    {
        _decorationView->setFrame({ 0, 0, _size.width, _size.height });
        _decorationView->layout();
    }

    void Window::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect({ 0, 0, _size.width, _size.height }, Gfx::Color::white);
        _decorationView->redraw(context);
    }
}
