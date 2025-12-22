#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::CloseButton final : public View
    {
    public:
        static RefPtr<CloseButton> make();

        void redraw(RefPtr<Gfx::Context> context) override;

        void onPointerButtonPressed(const Event& event) override;
        void onPointerButtonReleased(const Event& event) override;

    private:
        CloseButton() = default;
        ~CloseButton() = default;

        bool _pressed { false };
    };

    inline RefPtr<Window::CloseButton> Window::CloseButton::make()
    {
        return adopt(new CloseButton());
    }
}
