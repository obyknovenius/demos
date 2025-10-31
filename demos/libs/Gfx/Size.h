#pragma once

namespace Gfx
{
    struct Size
    {
        int width { 0 };
        int height { 0 };

        Size inset(int x, int y) const
        {
            return { width - x, height - y };
        }
    };
}
