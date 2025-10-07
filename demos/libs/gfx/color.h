#pragma once

namespace gfx
{
    struct color
    {
        static const color black;
        static const color white;

        static const color red;
        static const color green;
        static const color blue;

        float r { 0.0f };
        float g { 0.0f };
        float b { 0.0f };
        float a { 1.0f };
    };
}
