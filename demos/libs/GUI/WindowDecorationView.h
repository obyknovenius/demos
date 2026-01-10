#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::DecorationView final : public View
    {
    public:
        static NonNull<RefPtr<DecorationView>> make(RefPtr<Window> window)
        {
            return adopt(new DecorationView(window));
        }

        RefPtr<TitleBar> titleBar() { return _titleBar; }
        RefPtr<View> contentView() { return _contentView; }

        void layout() override;
        void redraw(NonNull<RefPtr<Gfx::Context>> context) override;

        void onPointerButtonPressed(Event event) override;
        void onPointerButtonReleased(Event event) override;
        void onPointerEntered(Event event) override;
        void onPointerMoved(Event event) override;
        void onPointerLeft(Event event) override;
    private:
        DecorationView(RefPtr<Window> window);
        ~DecorationView() = default;

        Window::Edges resizeEdgesForPosition(const Gfx::Point& position) const;
        Cursor cursorForPosition(const Gfx::Point& position) const;

        RefPtr<TitleBar> _titleBar;
        RefPtr<View> _contentView;

        const int _borderThickness { 6 };
    };
}
