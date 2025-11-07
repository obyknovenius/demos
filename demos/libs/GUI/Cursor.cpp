#include "Cursor.h"

namespace GUI
{
    const Cursor Cursor::_defaultCursor {};
    const Cursor Cursor::_pointerCursor {};

    const Cursor* Cursor::_currentCursor = &_defaultCursor;
}
