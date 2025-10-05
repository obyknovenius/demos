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
    rect(point origin, size size) :
        x { origin.x },
        y { origin.y },
        width { size.width },
        height { size.height }
    {
    }
};

}
