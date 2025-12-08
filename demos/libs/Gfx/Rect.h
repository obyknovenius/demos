#pragma once

#include "Point.h"
#include "Size.h"

namespace Gfx
{
    struct Rect
    {
        Point origin { 0, 0 };
        Size size { 0, 0 };

        int minX() const
        {
            return origin.x;
        }

        int maxX() const
        {
            return origin.x + size.width;
        }

        int minY() const
        {
            return origin.y;
        }

        int maxY() const
        {
            return origin.y + size.height;
        }

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
