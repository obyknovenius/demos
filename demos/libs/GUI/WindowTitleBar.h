#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::TitleBar final : public View
    {
    public:
        static NonNull<RefPtr<TitleBar>> make()
        {
            return adopt(new TitleBar());
        }

        Gfx::Size intrinsicSize() override { return { -1, _height }; }

        void layout() override;
        void redraw(NonNull<RefPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(Event event) override;

    private:
        TitleBar();
        ~TitleBar();

        RefPtr<CloseButton> _closeButton;
        RefPtr<MaximizeButton> _maximizeButton;

        int _height { 26 };
    };
}
