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
        RefPtr<View> superview();
        void addSubview(NonnullRefPtr<View> subview);

        void setFrame(const Gfx::Rect& frame);
        const Gfx::Rect& frame() const;

        virtual Gfx::Size intrinsicSize() const;

        virtual void layout();
        virtual void redraw(NonnullRefPtr<Gfx::Context> context);

        RefPtr<View> hitTest(const Gfx::Point& point);

        virtual void onPointerButtonPressed();
        virtual void onPointerButtonReleased();
        virtual void onPointerEntered();
        virtual void onPointerMoved();
        virtual void onPointerLeft();

    protected:
        View() = default;
        virtual ~View() = default;

    protected:
        WeakPtr<View> _superview {};
        std::vector<NonnullRefPtr<View>> _subviews {};

        Gfx::Rect _frame {};
        Gfx::Rect _bounds {};
    };

    inline RefPtr<View> View::superview()
    {
        return _superview.strong();
    }

    inline const Gfx::Rect& View::frame() const
    {
        return _frame;
    }

    inline Gfx::Size View::intrinsicSize() const
    {
        return { -1, -1 };
    }

    inline void View::onPointerButtonPressed()
    {
    }

    inline void View::onPointerButtonReleased()
    {
    }

    inline void View::onPointerEntered()
    {
    }

    inline void View::onPointerMoved()
    {
    }

    inline void View::onPointerLeft()
    {
    }
}
