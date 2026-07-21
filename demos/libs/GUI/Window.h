#pragma once

#include "Cursor.h"
#include "Forward.h"
#include "View.h"
#include <Foundation/Foundation.h>
#include <Gfx/Forward.h>
#include <Gfx/Size.h>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace GUI
{
    class Window : public Object
    {
    public:
        enum class Edge
        {
            Top = 1 << 0,
            Bottom = 1 << 1,
            Left = 1 << 2,
            Right = 1 << 3,
        };

        using Edges = OptionSet<Edge>;

        static NonNull<StrongPtr<Window>> make();

        virtual void setMaximized(bool maximized) { _maximized = maximized; }
        bool maximized() { return _maximized; }

        virtual void close();

        std::function<void()> onClose;

        virtual void beginMove() = 0;
        virtual void beginResize(Edges edges) = 0;

        void dispatchEvent(std::unique_ptr<const Event> event);

        virtual void setCursor(Cursor cursor) { _currentCursor = cursor; }
        Cursor currentCursor() { return _currentCursor; }
        void pushCursor(Cursor cursor);
        void popCursor();

    protected:
        class DecorationView;
        class TitleBar;
        class CloseButton;
        class MaximizeButton;

        Window(Gfx::Size size = { 800, 600 });
        ~Window();

        void layout();
        void redraw(NonNull<StrongPtr<Gfx::Context>> context);

        Gfx::Size _size;

        bool _maximized{ false };

        NonNull<StrongPtr<DecorationView>> _decorationView;

        Cursor _currentCursor{ Cursor::Default };
        std::vector<Cursor> _cursorStack;

        WeakPtr<View> _viewUnderPointer;
    };
}
