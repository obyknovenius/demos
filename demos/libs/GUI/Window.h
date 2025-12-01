#pragma once

#include "Cursor.h"
#include "View.h"
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
        enum class Edge {
            Top = 1 << 0,
            Bottom = 1 << 1,
            Left = 1 << 2,
            Right = 1 << 3,
        };

        using Edges = OptionSet<Edge>;

        static NonnullRefPtr<Window> make();

        virtual void beginMove() = 0;
        virtual void beginResize(Edges edges) = 0;

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
