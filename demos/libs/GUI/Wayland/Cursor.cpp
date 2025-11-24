#include "Cursor.h"

namespace GUI::Wayland
{
    const Cursor Cursor::_defaultCursor { WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT };
    const Cursor Cursor::_pointerCursor { WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_POINTER };

    Cursor::Cursor(wp_cursor_shape_device_v1_shape shape) :
        GUI::Cursor {},
        _shape { shape }
    {
    }
}
