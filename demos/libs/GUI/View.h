#pragma once

#include "Event.h"
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
        static NonnullRefPtr<View> make();

        RefPtr<Window> window();

        RefPtr<View> superview();
        void addSubview(NonnullRefPtr<View> subview);

        void setFrame(const Gfx::Rect& frame);
        const Gfx::Rect& frame() const;

        virtual Gfx::Size intrinsicSize() const;

        virtual void layout();
        virtual void redraw(NonnullRefPtr<Gfx::Context> context);

        RefPtr<View> hitTest(const Gfx::Point& point);

        virtual void onPointerButtonPressed(const Event& event);
        virtual void onPointerButtonReleased(const Event& event);
        virtual void onPointerEntered(const Event& event);
        virtual void onPointerMoved(const Event& event);
        virtual void onPointerLeft(const Event& event);
    protected:
        View() = default;
        virtual ~View() = default;

        WeakPtr<Window> _window {};

        WeakPtr<View> _superview {};
        std::vector<NonnullRefPtr<View>> _subviews {};

        Gfx::Rect _frame {};
        Gfx::Rect _bounds {};

    private:
        void _willBeMovedToWindow(const RefPtr<Window>& window);
    };

    inline NonnullRefPtr<View> View::make()
    {
        return adopt(*new View());
    }

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

    inline void View::onPointerButtonPressed(const Event& event)
    {
    }

    inline void View::onPointerButtonReleased(const Event& event)
    {
    }

    inline void View::onPointerEntered(const Event& event)
    {
    }

    inline void View::onPointerMoved(const Event& event)
    {
    }

    inline void View::onPointerLeft(const Event& event)
    {
    }
}
