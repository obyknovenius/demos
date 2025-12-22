#include "View.h"

#include "Window.h"

namespace GUI
{
    RefPtr<Window> View::window()
    {
        return _window.strong();
    }

    void View::addSubview(RefPtr<View> subview)
    {
        subview->_willBeMovedToWindow(_window.strong());
        subview->_superview = this;
        _subviews.push_back(subview);
    }

    void View::setFrame(const Gfx::Rect& frame)
    {
        _frame = frame;
        _bounds = { { 0, 0 }, frame.size };
    }

    void View::layout()
    {
        for (const auto& subview : _subviews)
            subview->layout();
    }

    void View::redraw(RefPtr<Gfx::Context> context)
    {
        for (const auto& subview : _subviews)
        {
            context->save();
            context->clip(subview->frame());
            context->translate(subview->frame().origin);
            subview->redraw(context);
            context->restore();
        }
    }

    RefPtr<View> View::hitTest(const Gfx::Point& point)
    {
        for (const auto& subview : _subviews)
            if (subview->frame().contains(point))
                return subview->hitTest(point - subview->frame().origin);
        return this;
    }

    void View::_willBeMovedToWindow(const RefPtr<Window>& window)
    {
        // TODO: Compare with existing window and early return if unchanged
        _window = window;
        for (const auto& subview : _subviews)
            subview->_willBeMovedToWindow(window);
    }
}
