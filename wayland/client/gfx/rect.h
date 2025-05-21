#pragma once

#include "point.h"
#include "size.h"

namespace gfx {

struct Rect
{
    float x {};
    float y {};
    float width {};
    float height {};

    Rect() = default;

    Rect(float x, float y, float width, float height)
        : x { x }
        , y { y }
        , width { width }
        , height { height }
    {
    }

    Rect(Point origin, Size size)
        : x { origin.x }
        , y { origin.y }
        , width { size.width }
        , height { size.height }
    {
    }

    auto min_x() const -> float
    {
        return x;
    }

    auto max_x() const -> float
    {
        return x + width;
    }

    auto min_y() const -> float
    {
        return y;
    }

    auto max_y() const -> float
    {
        return y + height;
    }

    auto origin() const -> Point
    {
        return { x, y };
    }

    auto size() const -> Size
    {
        return { width, height };
    }

    auto inset_by(float dx, float dy) const -> Rect
    {
        return { origin() + Point { dx, dy }, size() - Size { dx * 2, dy * 2 } };
    }
};

}
