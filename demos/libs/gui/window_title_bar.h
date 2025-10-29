#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::title_bar final : public view
    {
    public:
        static nonnull_ref_ptr<title_bar> make();

        gfx::size intrinsic_size() override { return { -1, m_height }; }

        void redraw(nonnull_ref_ptr<gfx::context> context) override;
    
    private:
        title_bar() = default;
        ~title_bar() = default;

        int m_height = 32;
    };

    inline nonnull_ref_ptr<window::title_bar> window::title_bar::make()
    {
        return adopt(*new title_bar());
    }
}
