#pragma once

#include "Event.h"
#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/Weakable.h>
#include <Gfx/Size.h>

namespace Platform
{
    class Display;

    class Window : public RefCounted, public Weakable
    {
    public:
        class Delegate : public RefCounted
        {
        public:
            virtual void layoutWindow(NonNull<RefPtr<Window>> window) {};
            virtual void drawWindow(NonNull<RefPtr<Window>> window) {};
            virtual void windowDidReceiveEvent(NonNull<RefPtr<Platform::Window>> window, Platform::Event event) {};
        };

        static NonNull<RefPtr<Window>> create(NonNull<RefPtr<Display>> display, Gfx::Size size = { 800, 600 });

        Gfx::Size size() const { return _size; }

        bool needsLayout() const { return _needsLayout; }
        virtual void setNeedsLayout() { _needsLayout = true; }

        bool needsDraw() const { return _needsDraw; }
        virtual void setNeedsDraw() { _needsDraw = true; }

        void receiveEvent(Event event);

        RefPtr<Delegate> delegate() const { return _delegate; }
        void setDelegate(RefPtr<Delegate> delegate) { _delegate = delegate; }

    protected:
        Window(Gfx::Size size) : _size { size } {};

        void layoutIfNeeded();
        void drawIfNeeded();

        virtual void layout();
        virtual void draw();

        Gfx::Size _size {};

        bool _needsLayout { false };
        bool _needsDraw { false };

        RefPtr<Delegate> _delegate;
    };

    inline void Window::layoutIfNeeded()
    {
        if (_needsLayout)
            layout();
    }

    inline void Window::drawIfNeeded()
    {
        if (_needsDraw)
            draw();
    }

    inline void Window::layout()
    {
        if (_delegate)
            _delegate->layoutWindow(this);
        _needsLayout = false;
    }

    inline void Window::draw()
    {
        if (_delegate)
            _delegate->drawWindow(this);
        _needsDraw = false;
    }

    inline void Window::receiveEvent(Event event)
    {
        if (_delegate)
            _delegate->windowDidReceiveEvent(this, event);
    }
}
