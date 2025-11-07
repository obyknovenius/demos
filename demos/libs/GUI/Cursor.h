#pragma once

namespace GUI
{
    class Cursor
    {
    public:
        static const Cursor& defaultCursor();
        static const Cursor& pointerCursor();

        static const Cursor& currentCursor();

        static void set(const Cursor& cursor);

    private:
        static const Cursor _defaultCursor;
        static const Cursor _pointerCursor;

        static const Cursor* _currentCursor;

        Cursor() = default;
        ~Cursor() = default;
    };

    inline const Cursor& Cursor::defaultCursor()
    {
        return _defaultCursor;
    }

    inline const Cursor& Cursor::pointerCursor()
    {
        return _pointerCursor;
    }

    inline void Cursor::set(const Cursor& cursor)
    {
        _currentCursor = &cursor;
    }

    inline const Cursor& Cursor::currentCursor()
    {
        return *_currentCursor;
    }
}
