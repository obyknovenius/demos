#pragma once

#include "Event.h"
#include <Foundation/EnableWeakPtr.h>
#include <Foundation/NonNull.h>
#include <Foundation/NonNullRefPtr.h>
#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>
#include <Foundation/WeakPtr.h>
#include <Gfx/Size.h>

namespace Platform
{
    class Display;

    class Window : public RefCounted, public EnableWeakPtr<Window>
    {
    public:
        class Delegate : public EnableWeakPtr<Delegate>
        {
        public:
            virtual void layoutWindow(NonNullRefPtr<Window> window) {};
            virtual void drawWindow(NonNullRefPtr<Window> window) {};
            virtual void windowDidReceiveEvent(NonNullRefPtr<Platform::Window> window, Platform::Event event) {};
        };

        static NonNullRefPtr<Window> create(NonNullRefPtr<Display> display, Gfx::Size size = { 800, 600 });

        Gfx::Size size() const { return _size; }

        bool needsLayout() const { return _needsLayout; }
        virtual void setNeedsLayout() { _needsLayout = true; }

        bool needsDraw() const { return _needsDraw; }
        virtual void setNeedsDraw() { _needsDraw = true; }

        void receiveEvent(Event event);

        WeakPtr<Delegate> delegate() const { return _delegate; }
        void setDelegate(WeakPtr<Delegate> delegate) { _delegate = delegate; }

    protected:
        Window(Gfx::Size size) : _size { size } {};

        void layoutIfNeeded();
        void drawIfNeeded();

        virtual void layout();
        virtual void draw();

        Gfx::Size _size {};

        bool _needsLayout { false };
        bool _needsDraw { false };

        WeakPtr<Delegate> _delegate;
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
