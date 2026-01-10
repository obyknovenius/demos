#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::CloseButton final : public View
    {
    public:
        static NonNull<RefPtr<CloseButton>> make();

        void redraw(NonNull<RefPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(const Event& event) override;
        void onPointerButtonReleased(const Event& event) override;

    private:
        CloseButton() = default;
        ~CloseButton() = default;

        bool _pressed { false };
    };

    inline NonNull<RefPtr<Window::CloseButton>> Window::CloseButton::make()
    {
        return adopt(new CloseButton());
    }
}
