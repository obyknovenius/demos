#include "WindowDecorationView.h"

#include "WindowTitleBar.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace GUI
{
    Window::DecorationView::DecorationView(RefPtr<Window> window) :
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

    void Window::DecorationView::redraw(NonNull<RefPtr<Gfx::Context>> context)
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

    void Window::DecorationView::onPointerButtonPressed(Event event)
    {
        if (auto window = _window.strong())
            if (auto edges = resizeEdgesForPosition(*event.position))
                window->beginResize(edges);
    }

    void Window::DecorationView::onPointerButtonReleased(Event event)
    {
        if (auto window = _window.strong())
        {
            auto cursor = event.pressedPointerButtons ? Cursor::Default : cursorForPosition(*event.position);
            window->setCursor(cursor);
        }
    }

    void Window::DecorationView::onPointerEntered(Event event)
    {
        if (auto window = _window.strong())
        {
            auto cursor = event.pressedPointerButtons ? Cursor::Default : cursorForPosition(*event.position);
            window->pushCursor(cursor);
        }
    }

    void Window::DecorationView::onPointerMoved(Event event)
    {
        if (auto window = _window.strong())
        {
            auto cursor = event.pressedPointerButtons ? Cursor::Default : cursorForPosition(*event.position);
            window->setCursor(cursor);
        }
    }

    void Window::DecorationView::onPointerLeft(Event event)
    {
        if (auto window = _window.strong())
            window->popCursor();
    }

    Window::Edges Window::DecorationView::resizeEdgesForPosition(Gfx::Point position) const
    {
        Window::Edges edges;
        if (position.y < _borderThickness)
            edges.add(Window::Edge::top);
        if (position.y >= _frame.size.height - _borderThickness)
            edges.add(Window::Edge::bottom);
        if (position.x < _borderThickness)
            edges.add(Window::Edge::left);
        if (position.x >= _frame.size.width - _borderThickness)
            edges.add(Window::Edge::right);
        return edges;
    }

    Cursor Window::DecorationView::cursorForPosition(Gfx::Point position) const
    {
        auto edges = resizeEdgesForPosition(position);
        if (edges.containsOnly(Window::Edge::top))
            return Cursor::NorthResize;
        if (edges.containsOnly(Window::Edge::bottom))
            return Cursor::SouthResize;
        if (edges.containsOnly(Window::Edge::left))
            return Cursor::WestResize;
        if (edges.containsOnly(Window::Edge::right))
            return Cursor::EastResize;
        if (edges.containsOnly(Window::Edge::top, Window::Edge::left))
            return Cursor::NorthWestResize;
        if (edges.containsOnly(Window::Edge::top, Window::Edge::right))
            return Cursor::NorthEastResize;
        if (edges.containsOnly(Window::Edge::bottom, Window::Edge::left))
            return Cursor::SouthWestResize;
        if (edges.containsOnly(Window::Edge::bottom, Window::Edge::right))
            return Cursor::SouthEastResize;
        return Cursor::Default;
    }
}
