#pragma once

namespace gfx {

struct Size
{
    float width {};
    float height {};

    auto operator+(Size const& other) const -> Size
    {
        return { width + other.width, height + other.height };
    }

    auto operator-(Size const& other) const -> Size
    {
        return { width - other.width, height - other.height };
    }
};

}
