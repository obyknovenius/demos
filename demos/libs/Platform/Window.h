#pragma once

#include "Event.h"
#include <Foundation/NonNull.h>
#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>
#include <Foundation/WeakPtr.h>
#include <Gfx/Size.h>

namespace Platform
{
    class Window : public Object
    {
    public:
        class Delegate
        {
        public:
            virtual void layoutWindow(StrongPtr<Window> window) {};
            virtual void drawWindow(StrongPtr<Window> window) {};
            virtual void windowDidReceiveEvent(StrongPtr<Platform::Window> window, Platform::Event event) {};
        };

        static NonNull<StrongPtr<Window>> create(Gfx::Size size = { 800, 600 });

        Gfx::Size size() const { return _size; }

        bool needsLayout() const { return _needsLayout; }
        virtual void setNeedsLayout() { _needsLayout = true; }

        bool needsDraw() const { return _needsDraw; }
        virtual void setNeedsDraw() { _needsDraw = true; }

        void receiveEvent(Event event);

        WeakPtr<Delegate> delegate() const { return _delegate; }
        void setDelegate(WeakPtr<Delegate> delegate) { _delegate = delegate; }

    protected:
        Window(Gfx::Size size) : _size(size) {};

        void layoutIfNeeded();
        void drawIfNeeded();

        virtual void layout();
        virtual void draw();

        Gfx::Size _size = { 0, 0 };

        bool _needsLayout = false;
        bool _needsDraw = false;

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
        if (StrongPtr delegate = _delegate)
            delegate->layoutWindow(this);
        _needsLayout = false;
    }

    inline void Window::draw()
    {
        if (StrongPtr delegate = _delegate)
            delegate->drawWindow(this);
        _needsDraw = false;
    }

    inline void Window::receiveEvent(Event event)
    {
        if (StrongPtr delegate = _delegate)
            delegate->windowDidReceiveEvent(this, event);
    }
}
