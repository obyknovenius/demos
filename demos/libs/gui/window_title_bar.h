#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::title_bar final : public view
    {
    public:
        title_bar(const gfx::rect& frame) : view { frame }
        {
        }

        ~title_bar() = default;

        void redraw(nonnull_ref_ptr<gfx::context> context) override;
    };
}
