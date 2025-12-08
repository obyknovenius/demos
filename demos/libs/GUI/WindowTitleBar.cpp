#include "WindowTitleBar.h"

#include "Event.h"
#include "WindowCloseButton.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace GUI
{
    Window::TitleBar::TitleBar() :
        View {},
        _closeButton { CloseButton::make() }
    {
        addSubview(_closeButton);
    }

    Window::TitleBar::~TitleBar() = default;

    void Window::TitleBar::layout()
    {
        _closeButton->setFrame({
            origin: { 0, 0 },
            size: _closeButton->intrinsicSize()
        });
    }

    void Window::TitleBar::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect(_bounds, Gfx::Color::white);

        auto closeButtonWidth = _closeButton->intrinsicSize().width;
        auto closeButtonMargin = 4;
        for (int i = 0, y = 2; i < 6; ++i, y += 4)
            context->strokeLine({ closeButtonWidth + closeButtonMargin, y }, { _bounds.size.width, y }, Gfx::Color::black, 2.0f);

        View::redraw(context);
    }

    void Window::TitleBar::onPointerButtonPressed(const Event& event)
    {
        if (auto window = _window.strong())
            window->beginMove();
    }
}
