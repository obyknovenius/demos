#pragma once

namespace gfx {

struct Point
{
    float x {};
    float y {};

    auto operator+(Point const& other) const -> Point
    {
        return { x + other.x, y + other.y };
    }

    auto operator-(Point const& other) const -> Point
    {
        return { x - other.x, y - other.y };
    }
};

}
