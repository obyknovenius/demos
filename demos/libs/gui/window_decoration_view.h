#pragma once

#include "view.h"
#include "window.h"

namespace gui
{
    class window::decoration_view final : public view
    {
    public:
        static nonnull_ref_ptr<decoration_view> make();

        nonnull_ref_ptr<title_bar> get_title_bar() { return m_title_bar; }

        void layout() override;
        void redraw(nonnull_ref_ptr<gfx::context> context) override;

    private:
        decoration_view();
        ~decoration_view() = default;

        nonnull_ref_ptr<title_bar> m_title_bar;

        const int m_border_thickness { 4 };
    };

    inline nonnull_ref_ptr<window::decoration_view> window::decoration_view::make()
    {
        return adopt(*new decoration_view());
    }
}
