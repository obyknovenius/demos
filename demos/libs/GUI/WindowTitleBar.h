#pragma once

#include "View.h"
#include "Window.h"
#include <Foundation/Foundation.h>

namespace GUI
{
    class Window::TitleBar final : public View
    {
    public:
        static NonNull<StrongPtr<TitleBar>> make()
        {
            return StrongPtr<TitleBar>::adopt(new TitleBar());
        }

        Gfx::Size intrinsicSize() override { return { -1, _height }; }

        void layout() override;
        void redraw(NonNull<StrongPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(Event event) override;

    private:
        TitleBar();
        ~TitleBar();

        StrongPtr<CloseButton> _closeButton;
        StrongPtr<MaximizeButton> _maximizeButton;

        int _height{ 26 };
    };
}
