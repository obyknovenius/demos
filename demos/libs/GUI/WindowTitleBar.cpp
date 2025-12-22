#include "WindowTitleBar.h"

#include "Event.h"
#include "WindowCloseButton.h"
#include "WindowMaximizeButton.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace GUI
{
    Window::TitleBar::TitleBar() :
        View {},
        _closeButton { CloseButton::make() },
        _maximizeButton { MaximizeButton::make() }
    {
        addSubview(_closeButton);
        addSubview(_maximizeButton);
    }

    Window::TitleBar::~TitleBar() = default;

    void Window::TitleBar::layout()
    {
        int buttonSize = 22;

        _closeButton->setFrame({
            origin: { 0, 0 },
            size: { buttonSize, buttonSize }
        });

        _maximizeButton->setFrame({
            origin: { _frame.size.width - buttonSize, 0 },
            size: { buttonSize, buttonSize }
        });
    }

    void Window::TitleBar::redraw(RefPtr<Gfx::Context> context)
    {
        context->fillRect(_bounds, Gfx::Color::white);

        int closeButtonWidth = _closeButton->frame().size.width;
        int maximizeButtonWidth = _maximizeButton->frame().size.width;
        int buttonMargin = 4;

        for (int i = 0, y = 1; i < 6; ++i, y += 4)
        {
            Gfx::Point from { closeButtonWidth + buttonMargin, y };
            Gfx::Point to { _bounds.size.width - maximizeButtonWidth - buttonMargin, y };
            context->strokeLine(from, to, Gfx::Color::black, 2.0f );
        }

        View::redraw(context);
    }

    void Window::TitleBar::onPointerButtonPressed(const Event& event)
    {
        if (auto window = _window.strong())
            window->beginMove();
    }
}
