#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::MaximizeButton final : public View
    {
    public:
        static NonnullRefPtr<MaximizeButton> make();

        void redraw(NonnullRefPtr<Gfx::Context> context) override;

        void onPointerButtonPressed(const Event& event) override;
        void onPointerButtonReleased(const Event& event) override;

    private:
        MaximizeButton() = default;
        ~MaximizeButton() = default;

        bool _pressed { false };
    };

    inline NonnullRefPtr<Window::MaximizeButton> Window::MaximizeButton::make()
    {
        return adopt(*new MaximizeButton());
    }
}
