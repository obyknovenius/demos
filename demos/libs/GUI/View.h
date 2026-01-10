#pragma once

#include "Event.h"
#include "Forward.h"
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
    class View : public Weakable
    {
    public:
        static NonNull<RefPtr<View>> make()
        {
            return adopt(new View());
        }

        RefPtr<Window> window();

        RefPtr<View> superview() { return _superview.strong(); }
        void addSubview(RefPtr<View> subview);

        void setFrame(Gfx::Rect frame);
        Gfx::Rect frame() { return _frame; }

        virtual Gfx::Size intrinsicSize() { return { -1, -1 }; }

        virtual void layout();
        virtual void redraw(NonNull<RefPtr<Gfx::Context>> context);

        RefPtr<View> hitTest(Gfx::Point point);

        virtual void onPointerButtonPressed(Event event) {}
        virtual void onPointerButtonReleased(Event event) {}
        virtual void onPointerEntered(Event event) {}
        virtual void onPointerMoved(Event event) {}
        virtual void onPointerLeft(Event event) {}
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
}
