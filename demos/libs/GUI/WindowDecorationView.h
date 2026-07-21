#pragma once

#include "View.h"
#include "Window.h"
#include <Foundation/Foundation.h>

namespace GUI
{
    class Window::DecorationView final : public View
    {
    public:
        static NonNull<StrongPtr<DecorationView>> make(StrongPtr<Window> window)
        {
            return StrongPtr<DecorationView>::adopt(new DecorationView(window));
        }

        StrongPtr<TitleBar> titleBar() { return _titleBar; }
        StrongPtr<View> contentView() { return _contentView; }

        void layout() override;
        void redraw(NonNull<StrongPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(Event event) override;
        void onPointerButtonReleased(Event event) override;
        void onPointerEntered(Event event) override;
        void onPointerMoved(Event event) override;
        void onPointerLeft(Event event) override;

    private:
        DecorationView(StrongPtr<Window> window);
        ~DecorationView() = default;

        Window::Edges resizeEdgesForPosition(Gfx::Point position) const;
        Cursor cursorForPosition(Gfx::Point position) const;

        StrongPtr<TitleBar> _titleBar;
        StrongPtr<View> _contentView;

        const int _borderThickness{ 6 };
    };
}
