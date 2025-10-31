#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::title_bar final : public view
    {
    public:
        static NonnullRefPtr<title_bar> make();

        Gfx::Size intrinsic_size() override { return { -1, m_height }; }

        void redraw(NonnullRefPtr<Gfx::Context> context) override;
    
    private:
        title_bar() = default;
        ~title_bar() = default;

        int m_height = 32;
    };

    inline NonnullRefPtr<window::title_bar> window::title_bar::make()
    {
        return adopt(*new title_bar());
    }
}
