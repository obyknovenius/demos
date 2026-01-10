#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::MaximizeButton final : public View
    {
    public:
        static NonNull<RefPtr<MaximizeButton>> make();

        void redraw(NonNull<RefPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(const Event& event) override;
        void onPointerButtonReleased(const Event& event) override;

    private:
        MaximizeButton() = default;
        ~MaximizeButton() = default;

        bool _pressed { false };
    };

    inline NonNull<RefPtr<Window::MaximizeButton>> Window::MaximizeButton::make()
    {
        return adopt(new MaximizeButton());
    }
}
