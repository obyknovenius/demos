#pragma once

#include "Surface.h"

namespace Gfx::Wayland
{
    class Toplevel final : public Surface
    {
    public:
        static NonNull<RefPtr<Toplevel>> create(NonNull<RefPtr<Display>> display)
        {
            return adopt(new Toplevel(display));
        }

    private:
        static const xdg_toplevel_listener _xdgToplevelListener;

        Toplevel(NonNull<RefPtr<Display>> display);
        ~Toplevel() override;

        void onXdgToplevelConfigure(xdg_toplevel* xdgToplevel, int32_t width, int32_t height, wl_array* states);
        void onXdgToplevelClose(xdg_toplevel* xdgToplevel);

        xdg_toplevel* _xdgToplevel {};
    };
}
