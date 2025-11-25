#pragma once

#include "Cursor.h"
#include "View.h"
#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <functional>
#include <Gfx/Size.h>
#include <memory>
#include <optional>

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
        static NonnullRefPtr<Window> make();

        bool shouldStartMove(const Gfx::Point& pointer_position) const;

        void dispatchEvent(std::unique_ptr<const Event> event);

        virtual void close();

        std::function<void()> onClose;

        void setCursor(Cursor cursor) { _currentCursor = cursor; }
        Cursor currentCursor() const { return _currentCursor; }

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

        WeakPtr<View> _viewUnderPointer;
    };
}
