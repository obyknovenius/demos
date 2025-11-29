#include "WindowDecorationView.h"

#include "WindowTitleBar.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace GUI
{
    Window::DecorationView::DecorationView(NonnullRefPtr<Window> window) :
        View {},
        _titleBar { TitleBar::make() },
        _contentView { View::make() }
    {
        _window = window;
        addSubview(_titleBar);
        addSubview(_contentView);
    }

    void Window::DecorationView::layout()
    {
        auto titleBarHeight = _titleBar->intrinsicSize().height;
        _titleBar->setFrame({
            origin: { _borderThickness, _borderThickness },
            size: { _frame.size.width - 2 * _borderThickness, titleBarHeight }
        });
        _contentView->setFrame({
            origin: { _borderThickness, titleBarHeight },
            size: { _frame.size.width - 2 * _borderThickness, _frame.size.height - titleBarHeight - _borderThickness }
        });

        View::layout();
    }

    void Window::DecorationView::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect(_bounds, Gfx::Color::white);

        int lineWidth = 2;
        auto titleBarHeight = _titleBar->intrinsicSize().height;

        Gfx::Rect innerBorderRect = {
            origin: { _borderThickness, _borderThickness + titleBarHeight },
            size: { _bounds.size.width - 2 * _borderThickness, _bounds.size.height - titleBarHeight - 2 * _borderThickness }
        };
        context->strokeRect(innerBorderRect.inset(lineWidth / 2), Gfx::Color::black, lineWidth);

        auto outerBorderRect = _bounds;
        context->strokeRect(outerBorderRect.inset(lineWidth / 2), Gfx::Color::black, lineWidth);

        View::redraw(context);
    }

    void Window::DecorationView::onPointerButtonPressed(const Event& event)
    {
        if (auto window = _window.strong())
            window->close();
    }

    void Window::DecorationView::onPointerEntered(const Event& event)
    {
        if (auto window = _window.strong())
        {
            auto cursor = cursorForPosition(*event.position);
            window->pushCursor(cursor);
        }
    }

    void Window::DecorationView::onPointerMoved(const Event& event)
    {
        if (auto window = _window.strong())
        {
            auto cursor = cursorForPosition(*event.position);
            window->setCursor(cursor);
        }
    }

    void Window::DecorationView::onPointerLeft(const Event& event)
    {
        if (auto window = _window.strong())
            window->popCursor();
    }

    Cursor Window::DecorationView::cursorForPosition(const Gfx::Point& position) const
    {
        if (position.y < _borderThickness)
            return Cursor::NorthResize;
        if (position.y >= _frame.size.height - _borderThickness)
            return Cursor::SouthResize;
        if (position.x < _borderThickness)
            return Cursor::WestResize;
        if (position.x >= _frame.size.width - _borderThickness)
            return Cursor::EastResize;
        return Cursor::Default;
    }
}
