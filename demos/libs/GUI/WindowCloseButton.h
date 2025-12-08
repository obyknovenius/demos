#pragma once

#include "View.h"
#include "Window.h"

namespace GUI
{
    class Window::CloseButton final : public View
    {
    public:
        static NonnullRefPtr<CloseButton> make();

        Gfx::Size intrinsicSize() const override;

        void redraw(NonnullRefPtr<Gfx::Context> context) override;

        void onPointerButtonPressed(const Event& event) override;
        void onPointerButtonReleased(const Event& event) override;

    private:
        CloseButton() = default;
        ~CloseButton() = default;

        int _size { 22 };

        bool _pressed { false };
    };

    inline NonnullRefPtr<Window::CloseButton> Window::CloseButton::make()
    {
        return adopt(*new CloseButton());
    }

    inline Gfx::Size Window::CloseButton::intrinsicSize() const
    {
        return { _size, _size };
    }
}
