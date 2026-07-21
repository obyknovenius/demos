#pragma once

#include "View.h"
#include "Window.h"
#include <Foundation/Foundation.h>

namespace GUI
{
    class Window::CloseButton final : public View
    {
    public:
        static NonNull<StrongPtr<CloseButton>> make()
        {
            return StrongPtr<CloseButton>::adopt(new CloseButton());
        }

        void redraw(NonNull<StrongPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(Event event) override;
        void onPointerButtonReleased(Event event) override;

    private:
        CloseButton() = default;
        ~CloseButton() = default;

        bool _pressed{ false };
    };
}
