#pragma once

#include <Core/RefPtr.h>
#include <Core/WeakPtr.h>
#include <Core/Weakable.h>
#include <Gfx/Color.h>
#include <Gfx/Context.h>
#include <Gfx/Rect.h>
#include <memory>
#include <vector>

namespace GUI
{
    struct Event;

    class View : public Weakable
    {
    public:
        RefPtr<View> getSuperview();
        void addSubview(NonnullRefPtr<View> subview);

        void setFrame(const Gfx::Rect& frame);
        const Gfx::Rect& getFrame() const;

        virtual Gfx::Size intrinsicSize() const;

        virtual void layout();
        virtual void redraw(NonnullRefPtr<Gfx::Context> context);

        RefPtr<View> hitTest(const Gfx::Point& point);

        virtual void onButtonPressed(std::unique_ptr<const Event> event);

    protected:
        View() = default;
        virtual ~View() = default;

    protected:
        WeakPtr<View> _superview {};
        std::vector<NonnullRefPtr<View>> _subviews {};

        Gfx::Rect _frame {};
        Gfx::Rect _bounds {};
    };

    inline RefPtr<View> View::getSuperview()
    {
        return _superview.strong();
    }

    inline const Gfx::Rect& View::getFrame() const
    {
        return _frame;
    }

    inline Gfx::Size View::intrinsicSize() const
    {
        return { -1, -1 };
    }

    inline void View::onButtonPressed(std::unique_ptr<const Event> event)
    {
    }
}
