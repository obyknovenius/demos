#pragma once

namespace gfx {

struct size {
    int width { 0 };
    int height { 0 };

    auto inset(int x, int y) const -> size
    {
        return { width - x, height - y };
    }
};

}
