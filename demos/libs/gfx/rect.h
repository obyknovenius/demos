#pragma once

#include <gfx/point.h>
#include <gfx/size.h>

namespace gfx {

struct rect {
    int x { 0 };
    int y { 0 };
    int width { 0 };
    int height { 0 };

    rect() = default;

    rect(int x, int y, int width, int height) :
        x { x },
        y { y },
        width { width },
        height { height }
    {
    }

    rect(point origin, size size) :
        x { origin.x },
        y { origin.y },
        width { size.width },
        height { size.height }
    {
    }

    auto inset(int d) const -> rect
    {
        return { x + d, y + d, width - 2 * d, height - 2 * d };
    }
};

}
