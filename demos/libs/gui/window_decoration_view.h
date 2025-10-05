#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::decoration_view final : public view
    {
    public:
        decoration_view(const gfx::rect& frame) : view { frame }
        {
        }

        ~decoration_view() = default;

        void redraw(nonnull_ref_ptr<gfx::context> context) override;
    };
}
