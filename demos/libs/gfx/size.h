#pragma once

namespace gfx
{
    struct size
    {
        int width { 0 };
        int height { 0 };

        size inset(int x, int y) const
        {
            return { width - x, height - y };
        }
    };
}
