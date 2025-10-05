#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::decoration_view final : public view
    {
    public:
        static auto create(const gfx::rect& frame) -> nonnull_ref_ptr<decoration_view>
        {
            return adopt(*new decoration_view(frame));
        }

        auto redraw(nonnull_ref_ptr<gfx::context> context) -> void override;

    private:
        decoration_view(const gfx::rect& frame) : view { frame } {}
        ~decoration_view() = default;
    };
}
