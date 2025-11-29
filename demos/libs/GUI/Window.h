#pragma once

#include "Cursor.h"
#include "View.h"
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
        enum class Edge {
            Top,
            Bottom,
            Left,
            Right,
        };

        static NonnullRefPtr<Window> make();

        virtual void beginResize(Edge edge) = 0;

        bool shouldStartMove(const Gfx::Point& pointer_position) const;

        void dispatchEvent(std::unique_ptr<const Event> event);

        virtual void close();

        std::function<void()> onClose;

        virtual void setCursor(Cursor cursor);
        void pushCursor(Cursor cursor);
        void popCursor();
        Cursor currentCursor() const;

    protected:
        class DecorationView;
        class TitleBar;

        Window(const Gfx::Size& size = { 800, 600 });
        ~Window();

        void layout();
        void redraw(NonnullRefPtr<Gfx::Context> context);

        Gfx::Size _size;

        NonnullRefPtr<DecorationView> _decorationView;

        Cursor _currentCursor { Cursor::Default };
        std::vector<Cursor> _cursorStack;

        WeakPtr<View> _viewUnderPointer;
    };

    inline void Window::setCursor(Cursor cursor)
    {
        _currentCursor = cursor;
    }

    inline Cursor Window::currentCursor() const
    {
        return _currentCursor;
    }
}
