#pragma once

#include "view.h"
#include "window.h"
#include <core/weak_ptr.h>

namespace gui
{
    class window::decoration_view final : public view
    {
    public:
        static nonnull_ref_ptr<decoration_view> make(const nonnull_ref_ptr<window>& window);

        nonnull_ref_ptr<title_bar> get_title_bar() { return m_title_bar; }

        void layout() override;
        void redraw(nonnull_ref_ptr<gfx::context> context) override;

        void on_button_pressed(std::unique_ptr<const event> event) override;

    private:
        decoration_view(nonnull_ref_ptr<window> window);
        ~decoration_view() = default;

        weak_ptr<window> m_window;

        nonnull_ref_ptr<title_bar> m_title_bar;

        const int m_border_thickness { 4 };
    };

    inline nonnull_ref_ptr<window::decoration_view> window::decoration_view::make(const nonnull_ref_ptr<window>& window)
    {
        return adopt(*new decoration_view(window));
    }
}
