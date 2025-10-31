#pragma once

namespace Gfx
{
    struct Color
    {
        static const Color black;
        static const Color white;

        static const Color red;
        static const Color green;
        static const Color blue;

        float r { 0.0f };
        float g { 0.0f };
        float b { 0.0f };
        float a { 1.0f };
    };
}
