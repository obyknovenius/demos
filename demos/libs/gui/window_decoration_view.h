#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::decoration_view final : public view
    {
    public:
        decoration_view(const gfx::rect& frame);

        ~decoration_view() = default;

        nonnull_ref_ptr<title_bar> get_title_bar() { return m_title_bar; }

        void layout() override;
        void redraw(nonnull_ref_ptr<gfx::context> context) override;

    private:
        nonnull_ref_ptr<title_bar> m_title_bar;
    };
}
