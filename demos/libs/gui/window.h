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
        auto dispatch_event(std::unique_ptr<const event> event) -> void;

        virtual auto close() -> void;

        std::function<void()> on_close;

    protected:
        class decoration_view;

        window(const gfx::size& size = { 800, 600 });
        ~window();

        auto redraw(nonnull_ref_ptr<gfx::context> context) -> void;

        gfx::size m_size;

        nonnull_ref_ptr<decoration_view> m_decoration_view;
    };
}
