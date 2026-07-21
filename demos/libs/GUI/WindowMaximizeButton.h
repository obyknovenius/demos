#pragma once

#include "View.h"
#include "Window.h"
#include <Foundation/Foundation.h>

namespace GUI
{
    class Window::MaximizeButton final : public View
    {
    public:
        static NonNull<StrongPtr<MaximizeButton>> make()
        {
            return StrongPtr<MaximizeButton>::adopt(new MaximizeButton());
        }

        void redraw(NonNull<StrongPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(Event event) override;
        void onPointerButtonReleased(Event event) override;

    private:
        MaximizeButton() = default;
        ~MaximizeButton() = default;

        bool _pressed{ false };
    };
}
