#pragma once

#include <Core/RefPtr.h>
#include <Core/WeakPtr.h>
#include <Core/Weakable.h>
#include <Gfx/Color.h>
#include <Gfx/Context.h>
#include <Gfx/Rect.h>
#include <memory>
#include <vector>

namespace gui
{
    class event;

    class view : public Weakable
    {
    public:
        RefPtr<view> get_superview();
        void add_subview(NonnullRefPtr<view> subview);

        void set_frame(const Gfx::Rect& frame);
        Gfx::Rect get_frame() const { return m_frame; }

        virtual Gfx::Size intrinsic_size() { return { -1, -1 }; }

        virtual void layout();
        virtual void redraw(NonnullRefPtr<Gfx::Context> context);

        RefPtr<view> hit_test(const Gfx::Point& point);

        virtual void on_button_pressed(std::unique_ptr<const event> event) {};

    protected:
        view() = default;
        virtual ~view() = default;

    protected:
        WeakPtr<view> m_superview {};
        std::vector<NonnullRefPtr<view>> m_subviews {};

        Gfx::Rect m_frame {};
        Gfx::Rect m_bounds {};
    };

    inline RefPtr<view> view::get_superview()
    {
        return m_superview.strong();
    }

    inline void view::add_subview(NonnullRefPtr<view> subview)
    {
        subview->m_superview = this;
        m_subviews.push_back(subview);
    }
}
