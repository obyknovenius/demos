#pragma once

#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Gfx/Size.h>

namespace Platform
{
    class Display;

    class Window : public RefCounted
    {
    public:
        static NonNull<RefPtr<Window>> create(NonNull<RefPtr<Display>> display, Gfx::Size size = { 800, 600 });

        Gfx::Size size() const { return _size; }

        bool needsLayout() const { return _needsLayout; }
        void setNeedsLayout() { _needsLayout = true; }

        bool needsRedraw() const { return _needsRedraw; }
        void setNeedsRedraw() { _needsRedraw = true; }

    protected:
        Window(Gfx::Size size) : _size { size } {};

        void layoutIfNeeded();
        void redrawIfNeeded();

        virtual void layout() = 0;
        virtual void draw() = 0;

        Gfx::Size _size {};

        bool _needsLayout { false };
        bool _needsRedraw { false };
    };

    inline void Window::layoutIfNeeded()
    {
        if (_needsLayout)
            layout();
    }

    inline void Window::redrawIfNeeded()
    {
        if (_needsRedraw)
            draw();
    }
}
