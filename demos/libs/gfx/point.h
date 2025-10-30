#pragma once

namespace gfx
{
    struct point
    {
        int x { 0 };
        int y { 0 };

        point operator-(const point& other) const
        {
            return { x - other.x, y - other.y };
        }
    };
}
