#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::DecorationView final : public View
    {
    public:
        static NonNull<RefPtr<DecorationView>> make(RefPtr<Window> window);

        RefPtr<TitleBar> titleBar();
        RefPtr<View> contentView();

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

    inline NonNull<RefPtr<Window::DecorationView>> Window::DecorationView::make(RefPtr<Window> window)
    {
        return adopt(new DecorationView(window));
    }

    inline RefPtr<Window::TitleBar> Window::DecorationView::titleBar()
    {
        return _titleBar;
    }

    inline RefPtr<View> Window::DecorationView::contentView()
    {
        return _contentView;
    }
}
