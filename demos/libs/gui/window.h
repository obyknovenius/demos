#pragma once

#include "view.h"
#include <core/ref_counted.h>
#include <gfx/size.h>
#include <functional>
#include <memory>

namespace gfx {
    class context;
    struct rect;
}

namespace gui {

class decoration_view;
struct event;

class window : public ref_counted
{
public:
    auto dispatch_event(std::unique_ptr<const event> event) -> void;

    virtual auto close() -> void;

    std::function<void()> on_close;

protected:
    class decoration_view final : public view
    {
        template<typename T, class... Args>
        friend nonnull_ref_ptr<T> core::make_ref_counted(Args&&...);

    public:
        auto redraw(nonnull_ref_ptr<gfx::context> context) -> void override;

    private:
        decoration_view(const gfx::rect& frame) : view { frame } {}
        ~decoration_view() = default;
    };

    window() = default;
    ~window() = default;

    auto redraw(nonnull_ref_ptr<gfx::context> context) -> void;

    gfx::size m_size { 800, 600 };

    nonnull_ref_ptr<decoration_view> m_decoration_view { make_ref_counted<decoration_view>(gfx::rect {0, 0, m_size.width, m_size.height}) };
};

}
