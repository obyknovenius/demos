#pragma once

#include "Point.h"
#include "Size.h"

namespace Gfx
{
    struct Rect
    {
        Point origin { 0, 0 };
        Size size { 0, 0 };

        bool contains(const Point& point) const
        {
            return point.x >= origin.x && point.x < origin.x + size.width && point.y >= origin.y && point.y < origin.y + size.height;
        }

        Rect inset(int inset) const
        {
            return { { origin.x + inset, origin.y + inset }, { size.width - 2 * inset, size.height - 2 * inset } };
        }
    };
}
