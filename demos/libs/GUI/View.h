#pragma once

#include "Event.h"
#include "Forward.h"
#include <Foundation/Foundation.h>
#include <Gfx/Color.h>
#include <Gfx/Context.h>
#include <Gfx/Rect.h>
#include <memory>
#include <vector>

namespace GUI
{
    class View : public Object
    {
    public:
        static NonNull<StrongPtr<View>> make()
        {
            return StrongPtr<View>::adopt(new View());
        }

        StrongPtr<Window> window();

        StrongPtr<View> superview() { return _superview; }
        void addSubview(StrongPtr<View> subview);

        void setFrame(Gfx::Rect frame);
        Gfx::Rect frame() { return _frame; }

        virtual Gfx::Size intrinsicSize() { return { -1, -1 }; }

        virtual void layout();
        virtual void redraw(NonNull<StrongPtr<Gfx::Context>> context);

        StrongPtr<View> hitTest(Gfx::Point point);

        virtual void onPointerButtonPressed(Event event) {}
        virtual void onPointerButtonReleased(Event event) {}
        virtual void onPointerEntered(Event event) {}
        virtual void onPointerMoved(Event event) {}
        virtual void onPointerLeft(Event event) {}

    protected:
        View() = default;
        ~View() = default;

        WeakPtr<Window> _window{};

        WeakPtr<View> _superview{};
        std::vector<StrongPtr<View>> _subviews{};

        Gfx::Rect _frame{};
        Gfx::Rect _bounds{};

    private:
        void _willBeMovedToWindow(StrongPtr<Window> window);
    };
}
