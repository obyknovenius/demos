#include "Cursor.h"

#include "Wayland/Cursor.h"

namespace GUI
{
    const Cursor& Cursor::defaultCursor()
    {
        return Wayland::Cursor::defaultCursor();
    }

    const Cursor& Cursor::pointerCursor()
    {
        return Wayland::Cursor::pointerCursor();
    }
}
