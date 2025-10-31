#pragma once

#include "view.h"
#include <Core/Weakable.h>
#include <functional>
#include <Gfx/Size.h>
#include <memory>

namespace gfx
{
    class context;
    struct rect;
    struct size;
}

namespace gui
{
    struct event;

    class window : public Weakable
    {
    public:
        static NonnullRefPtr<window> make();

        bool should_start_move(const Gfx::Point& pointer_position) const;

        void dispatch_event(std::unique_ptr<const event> event);

        virtual void close();

        std::function<void()> on_close;

    protected:
        class decoration_view;
        class title_bar;

        window(const Gfx::Size& size = { 800, 600 });
        ~window();

        void layout();
        void redraw(NonnullRefPtr<Gfx::Context> context);

        Gfx::Size m_size;

        NonnullRefPtr<decoration_view> m_decoration_view;
    };
}
