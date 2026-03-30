#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::CloseButton final : public View
    {
    public:
        static Core::NonNull<RefPtr<CloseButton>> make()
        {
            return adopt(new CloseButton());
        }

        void redraw(Core::NonNull<RefPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(Event event) override;
        void onPointerButtonReleased(Event event) override;

    private:
        CloseButton() = default;
        ~CloseButton() = default;

        bool _pressed { false };
    };
}
