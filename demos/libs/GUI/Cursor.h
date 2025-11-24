#pragma once

namespace GUI
{
    class Cursor
    {
    public:
        static const Cursor& defaultCursor();
        static const Cursor& pointerCursor();

    protected:
        Cursor() = default;
        virtual ~Cursor() = default;
    };
}
