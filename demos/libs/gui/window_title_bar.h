#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::title_bar final : public view
    {
    public:
        title_bar() = default;
        ~title_bar() = default;

        gfx::size intrinsic_size() override { return { -1, m_height }; }

        void redraw(nonnull_ref_ptr<gfx::context> context) override;
    
    private:
        int m_height = 32;
    };
}
