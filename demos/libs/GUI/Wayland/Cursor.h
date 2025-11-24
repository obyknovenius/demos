#pragma once

#include "../Cursor.h"
#include <cursor-shape-v1-client-protocol.h>

namespace GUI::Wayland
{
    class Cursor final : public GUI::Cursor
    {
    public:
        static const Cursor& defaultCursor();
        static const Cursor& pointerCursor();

        wp_cursor_shape_device_v1_shape shape() const { return _shape; }

    private:
        static const Cursor _defaultCursor;
        static const Cursor _pointerCursor;

        Cursor(wp_cursor_shape_device_v1_shape shape);
        ~Cursor() = default;

        wp_cursor_shape_device_v1_shape _shape;
    };

    inline const Cursor& Cursor::defaultCursor()
    {
        return _defaultCursor;
    }

    inline const Cursor& Cursor::pointerCursor()
    {
        return _pointerCursor;
    }
}
