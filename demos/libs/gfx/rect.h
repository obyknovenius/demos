#pragma once

#include <gfx/point.h>
#include <gfx/size.h>

namespace gfx
{
    struct rect
    {
        struct point origin { 0, 0 };
        struct size size { 0, 0 };

        bool contains(const point& point) const
        {
            return point.x >= origin.x && point.x < origin.x + size.width && point.y >= origin.y && point.y < origin.y + size.height;
        }

        rect inset(int inset) const
        {
            return { { origin.x + inset, origin.y + inset }, { size.width - 2 * inset, size.height - 2 * inset } };
        }
    };
}
