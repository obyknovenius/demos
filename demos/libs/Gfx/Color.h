#pragma once

namespace Gfx
{
    struct Color
    {
        static const Color Black;
        static const Color White;

        static const Color Red;
        static const Color Green;
        static const Color Blue;

        float r { 0.0f };
        float g { 0.0f };
        float b { 0.0f };
        float a { 1.0f };
    };
}
