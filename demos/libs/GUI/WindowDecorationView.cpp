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
            if (auto edges = resizeEdgesForPosition(*event.position))
                window->beginResize(edges);
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

    Window::Edges Window::DecorationView::resizeEdgesForPosition(const Gfx::Point& position) const
    {
        Window::Edges edges;
        if (position.y < _borderThickness)
            edges |= Window::Edge::Top;
        if (position.y >= _frame.size.height - _borderThickness)
            edges |= Window::Edge::Bottom;
        if (position.x < _borderThickness)
            edges |= Window::Edge::Left;
        if (position.x >= _frame.size.width - _borderThickness)
            edges |= Window::Edge::Right;
        return edges;
    }

    Cursor Window::DecorationView::cursorForPosition(const Gfx::Point& position) const
    {
        auto edges = resizeEdgesForPosition(position);
        if (edges == Window::Edge::Top)
            return Cursor::NorthResize;
        if (edges == Window::Edge::Bottom)
            return Cursor::SouthResize;
        if (edges == Window::Edge::Left)
            return Cursor::WestResize;
        if (edges == Window::Edge::Right)
            return Cursor::EastResize;
        if (edges == (Window::Edge::Top | Window::Edge::Left))
            return Cursor::NorthWestResize;
        if (edges == (Window::Edge::Top | Window::Edge::Right))
            return Cursor::NorthEastResize;
        if (edges == (Window::Edge::Bottom | Window::Edge::Left))
            return Cursor::SouthWestResize;
        if (edges == (Window::Edge::Bottom | Window::Edge::Right))
            return Cursor::SouthEastResize;
        return Cursor::Default;
    }
}
