#pragma once

#include "Cursor.h"
#include "View.h"
#include <Core/NonNull.h>
#include <Core/OptionSet.h>
#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <Gfx/Size.h>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace Gfx
{
    class Context;
    struct Rect;
    struct Size;
}

namespace GUI
{
    struct Event;

    class Window : public Weakable
    {
    public:
        enum class Edge
        {
            top = 1 << 0,
            bottom = 1 << 1,
            left = 1 << 2,
            right = 1 << 3,
        };

        using Edges = OptionSet<Edge>;

        static NonNull<RefPtr<Window>> make();

        virtual void setMaximized(bool maximized);
        bool maximized() const;

        virtual void close();

        std::function<void()> onClose;

        virtual void beginMove() = 0;
        virtual void beginResize(Edges edges) = 0;

        void dispatchEvent(std::unique_ptr<const Event> event);

        virtual void setCursor(Cursor cursor);
        void pushCursor(Cursor cursor);
        void popCursor();
        Cursor currentCursor() const;

    protected:
        class DecorationView;
        class TitleBar;
        class CloseButton;
        class MaximizeButton;

        Window(Gfx::Size size = { 800, 600 });
        ~Window();

        void layout();
        void redraw(NonNull<RefPtr<Gfx::Context>> context);

        Gfx::Size _size;

        bool _maximized { false };

        NonNull<RefPtr<DecorationView>> _decorationView;

        Cursor _currentCursor { Cursor::Default };
        std::vector<Cursor> _cursorStack;

        WeakPtr<View> _viewUnderPointer;
    };

    inline void Window::setMaximized(bool maximized)
    {
        _maximized = maximized;
    }

    inline bool Window::maximized() const
    {
        return _maximized;
    }

    inline void Window::setCursor(Cursor cursor)
    {
        _currentCursor = cursor;
    }

    inline Cursor Window::currentCursor() const
    {
        return _currentCursor;
    }
}
