#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::TitleBar final : public View
    {
    public:
        static NonnullRefPtr<TitleBar> make();

        Gfx::Size intrinsicSize() const override;

        void redraw(NonnullRefPtr<Gfx::Context> context) override;

        void onPointerButtonPressed(const Event& event) override;

    private:
        TitleBar() = default;
        ~TitleBar() = default;

        int _height = 26;
    };

    inline NonnullRefPtr<Window::TitleBar> Window::TitleBar::make()
    {
        return adopt(*new TitleBar());
    }

    inline Gfx::Size Window::TitleBar::intrinsicSize() const
    {
        return { -1, _height };
    }
}
