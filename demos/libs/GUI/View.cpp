#include "View.h"

namespace GUI
{
    void View::addSubview(NonnullRefPtr<View> subview)
    {
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

    void View::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        for (const auto& subview : _subviews)
        {
            context->save();
            context->clip(subview->getFrame());
            context->translate(subview->getFrame().origin);
            subview->redraw(context);
            context->restore();
        }
    }

    RefPtr<View> View::hitTest(const Gfx::Point& point)
    {
        for (const auto& subview : _subviews)
            if (subview->getFrame().contains(point))
                return subview->hitTest(point - subview->getFrame().origin);
        return this;
    }
}
