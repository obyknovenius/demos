#pragma once

#include "view.h"
#include <core/weakable.h>
#include <functional>
#include <gfx/size.h>
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

    class window : public weakable
    {
    public:
        static nonnull_ref_ptr<window> make();

        bool should_start_move(const gfx::point& pointer_position) const;

        void dispatch_event(std::unique_ptr<const event> event);

        virtual void close();

        std::function<void()> on_close;

    protected:
        class decoration_view;
        class title_bar;

        window(const gfx::size& size = { 800, 600 });
        ~window();

        void layout();
        void redraw(nonnull_ref_ptr<gfx::context> context);

        gfx::size m_size;

        nonnull_ref_ptr<decoration_view> m_decoration_view;
    };
}
