#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::decoration_view final : public view
    {
    public:
        static nonnull_ref_ptr<decoration_view> create(const gfx::rect& frame)
        {
            return adopt(*new decoration_view(frame));
        }

        void redraw(nonnull_ref_ptr<gfx::context> context) override;

    private:
        decoration_view(const gfx::rect& frame) : view { frame } {}
        ~decoration_view() = default;
    };
}
