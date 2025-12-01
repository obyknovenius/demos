#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::DecorationView final : public View
    {
    public:
        static NonnullRefPtr<DecorationView> make(const NonnullRefPtr<Window>& window);

        NonnullRefPtr<TitleBar> titleBar();
        NonnullRefPtr<View> contentView();

        void layout() override;
        void redraw(NonnullRefPtr<Gfx::Context> context) override;

        void onPointerButtonPressed(const Event& event) override;
        void onPointerEntered(const Event& event) override;
        void onPointerMoved(const Event& event) override;
        void onPointerLeft(const Event& event) override;

    private:
        DecorationView(NonnullRefPtr<Window> window);
        ~DecorationView() = default;

        Window::Edges resizeEdgesForPosition(const Gfx::Point& position) const;
        Cursor cursorForPosition(const Gfx::Point& position) const;

        NonnullRefPtr<TitleBar> _titleBar;
        NonnullRefPtr<View> _contentView;

        const int _borderThickness { 4 };
    };

    inline NonnullRefPtr<Window::DecorationView> Window::DecorationView::make(const NonnullRefPtr<Window>& window)
    {
        return adopt(*new DecorationView(window));
    }

    inline NonnullRefPtr<Window::TitleBar> Window::DecorationView::titleBar()
    {
        return _titleBar;
    }

    inline NonnullRefPtr<View> Window::DecorationView::contentView()
    {
        return _contentView;
    }
}
