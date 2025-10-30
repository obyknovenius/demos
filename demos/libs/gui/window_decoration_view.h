#pragma once

#include "view.h"
#include "window.h"
#include <Core/WeakPtr.h>

namespace gui
{
    class window::decoration_view final : public view
    {
    public:
        static NonnullRefPtr<decoration_view> make(const NonnullRefPtr<window>& window);

        NonnullRefPtr<title_bar> get_title_bar() { return m_title_bar; }

        void layout() override;
        void redraw(NonnullRefPtr<gfx::context> context) override;

        void on_button_pressed(std::unique_ptr<const event> event) override;

    private:
        decoration_view(NonnullRefPtr<window> window);
        ~decoration_view() = default;

        WeakPtr<window> m_window;

        NonnullRefPtr<title_bar> m_title_bar;

        const int m_border_thickness { 4 };
    };

    inline NonnullRefPtr<window::decoration_view> window::decoration_view::make(const NonnullRefPtr<window>& window)
    {
        return adopt(*new decoration_view(window));
    }
}
