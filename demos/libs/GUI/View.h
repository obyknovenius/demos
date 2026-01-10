#pragma once

#include "Event.h"
#include <Core/NonNull.h>
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
    class Window;
    struct Event;

    class View : public Weakable
    {
    public:
        static NonNull<RefPtr<View>> make();

        RefPtr<Window> window();

        RefPtr<View> superview();
        void addSubview(RefPtr<View> subview);

        void setFrame(Gfx::Rect frame);
        Gfx::Rect frame() const;

        virtual Gfx::Size intrinsicSize() const;

        virtual void layout();
        virtual void redraw(NonNull<RefPtr<Gfx::Context>> context);

        RefPtr<View> hitTest(Gfx::Point point);

        virtual void onPointerButtonPressed(Event event);
        virtual void onPointerButtonReleased(Event event);
        virtual void onPointerEntered(Event event);
        virtual void onPointerMoved(Event event);
        virtual void onPointerLeft(Event event);
    protected:
        View() = default;
        virtual ~View() = default;

        WeakPtr<Window> _window {};

        WeakPtr<View> _superview {};
        std::vector<RefPtr<View>> _subviews {};

        Gfx::Rect _frame {};
        Gfx::Rect _bounds {};

    private:
        void _willBeMovedToWindow(RefPtr<Window> window);
    };

    inline NonNull<RefPtr<View>> View::make()
    {
        return adopt(new View());
    }

    inline RefPtr<View> View::superview()
    {
        return _superview.strong();
    }

    inline Gfx::Rect View::frame() const
    {
        return _frame;
    }

    inline Gfx::Size View::intrinsicSize() const
    {
        return { -1, -1 };
    }

    inline void View::onPointerButtonPressed(Event event)
    {
    }

    inline void View::onPointerButtonReleased(Event event)
    {
    }

    inline void View::onPointerEntered(Event event)
    {
    }

    inline void View::onPointerMoved(Event event)
    {
    }

    inline void View::onPointerLeft(Event event)
    {
    }
}
