#pragma once

#include <Core/RefCounted.h>

namespace Gfx
{
    struct Color;
    struct Rect;
    struct Point;

    class Context : public RefCounted
    {
    public:
        virtual void save() = 0;
        virtual void restore() = 0;

        virtual void translate(const Point& offset) = 0;

        virtual void clip(const Rect& rect) = 0;

        virtual void strokeLine(const Point& from, const Point& to, const Color& color, float line_width = 1.0f) = 0;
        virtual void strokeRect(const Rect& rect, const Color& color, float line_width = 1.0f) = 0;
        virtual void fillRect(const Rect& rect, const Color& color) = 0;

    protected:
        Context() = default;
        virtual ~Context() = default;
    };
}
