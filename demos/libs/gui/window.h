#pragma once

#include "view.h"
#include <core/ref_counted.h>
#include <gfx/size.h>
#include <functional>
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

    class window : public ref_counted
    {
    public:
        window(const gfx::size& size = { 800, 600 });
        ~window();

        bool should_start_move(const gfx::point& pointer_position) const;

        void dispatch_event(std::unique_ptr<const event> event);

        virtual void close();

        std::function<void()> on_close;

    protected:
        class decoration_view;
        class title_bar;

        void layout();
        void redraw(nonnull_ref_ptr<gfx::context> context);

        gfx::size m_size;

        nonnull_ref_ptr<decoration_view> m_decoration_view;
    };
}
