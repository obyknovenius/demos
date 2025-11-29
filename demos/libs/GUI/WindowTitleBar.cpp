#include "WindowTitleBar.h"

#include "Event.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace GUI
{
    void Window::TitleBar::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        for (int i = 0, y = 2; i < 6; ++i, y += 4)
            context->strokeLine({ 0, y }, { _bounds.size.width, y }, Gfx::Color::black, 2.0f);
    }

    void Window::TitleBar::onPointerButtonPressed(const Event& event)
    {
        if (auto window = _window.strong())
            window->beginMove();
    }
}
