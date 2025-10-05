#pragma once

#include <core/ref_counted.h>
#include <gfx/size.h>
#include <functional>
#include <memory>

namespace gfx {
    class context;
}

namespace gui {

struct event;

class window : public ref_counted
{
public:
    auto dispatch_event(std::unique_ptr<const event> event) -> void;

    virtual auto close() -> void;

    std::function<void()> on_close;

protected:
    window() = default;
    virtual ~window() = default;

    auto redraw(nonnull_ref_ptr<gfx::context> context) -> void;

    gfx::size m_size { 800, 600 };
};

}
