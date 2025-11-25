#include "WindowDecorationView.h"

#include "WindowTitleBar.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace GUI
{
    Window::DecorationView::DecorationView(NonnullRefPtr<Window> window) :
        View {},
        _titleBar { TitleBar::make() }
    {
        _window = window;
        addSubview(_titleBar);
    }

    void Window::DecorationView::layout()
    {
        _titleBar->setFrame({
            { 0, 0 },
            { _frame.size.width, _titleBar->intrinsicSize().height }
        });

        View::layout();
    }

    void Window::DecorationView::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect(_frame, Gfx::Color::white);

        int lineWidth = 2;

        auto innerBorderRect = _frame.inset(_borderThickness);
        context->strokeRect(innerBorderRect.inset(lineWidth / 2), Gfx::Color::black, lineWidth);

        View::redraw(context);

        auto outerBorderRect = _frame;
        context->strokeRect(outerBorderRect.inset(lineWidth / 2), Gfx::Color::black, lineWidth);
    }

    void Window::DecorationView::onPointerButtonPressed()
    {
        if (auto window = _window.strong())
            window->close();
    }

    void Window::DecorationView::onPointerEntered()
    {
        if (auto window = _window.strong())
            window->pushCursor(Cursor::Pointer);
    }

    void Window::DecorationView::onPointerMoved()
    {
    }

    void Window::DecorationView::onPointerLeft()
    {
        if (auto window = _window.strong())
            window->popCursor();
    }
}
