#pragma once

namespace Gfx
{
    struct Point
    {
        int x { 0 };
        int y { 0 };

        Point operator-(const Point& other) const
        {
            return { x - other.x, y - other.y };
        }
    };
}
