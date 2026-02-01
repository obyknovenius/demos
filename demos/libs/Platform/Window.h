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
        class Delegate : public RefCounted
        {
        public:
            virtual void layout(NonNull<RefPtr<Window>> window) {};
            virtual void draw(NonNull<RefPtr<Window>> window) {};
        };

        static NonNull<RefPtr<Window>> create(NonNull<RefPtr<Display>> display, Gfx::Size size = { 800, 600 });

        Gfx::Size size() const { return _size; }

        bool needsLayout() const { return _needsLayout; }
        void setNeedsLayout() { _needsLayout = true; }

        bool needsRedraw() const { return _needsRedraw; }
        void setNeedsRedraw() { _needsRedraw = true; }

        RefPtr<Delegate> delegate() const { return _delegate; }
        void setDelegate(RefPtr<Delegate> delegate) { _delegate = delegate; }

    protected:
        Window(Gfx::Size size) : _size { size } {};

        void layoutIfNeeded();
        void redrawIfNeeded();

        virtual void layout();
        virtual void draw();

        Gfx::Size _size {};

        bool _needsLayout { false };
        bool _needsRedraw { false };

        RefPtr<Delegate> _delegate;
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

    inline void Window::layout()
    {
        if (_delegate)
            _delegate->layout(this);
        _needsLayout = false;
    }

    inline void Window::draw()
    {
        if (_delegate)
            _delegate->draw(this);
        _needsRedraw = false;
    }
}
