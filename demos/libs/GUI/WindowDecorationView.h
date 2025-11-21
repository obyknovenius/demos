#pragma once

#include "View.h"
#include "Window.h"
#include <Core/WeakPtr.h>

namespace GUI
{
    class Window::DecorationView final : public View
    {
    public:
        static NonnullRefPtr<DecorationView> make(const NonnullRefPtr<Window>& window);

        NonnullRefPtr<TitleBar> titleBar();

        void layout() override;
        void redraw(NonnullRefPtr<Gfx::Context> context) override;

        void onPointerButtonPressed() override;

        void onPointerEntered() override;
        void onPointerMoved() override;
        void onPointerLeft() override;

    private:
        DecorationView(NonnullRefPtr<Window> window);
        ~DecorationView() = default;

        WeakPtr<Window> _window;

        NonnullRefPtr<TitleBar> _titleBar;

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
}
