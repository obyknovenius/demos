#pragma once

#include "Surface.h"

namespace GUI::Wayland
{
    class Toplevel final : public Surface
    {
    public:
        static NonNull<RefPtr<Toplevel>> make(RefPtr<Display> display) { return adopt(new Toplevel(display)); }

    private:
        static const xdg_toplevel_listener _xdgToplevelListener;

        Toplevel(NonNull<RefPtr<Display>> display);
        ~Toplevel() override;

        void onToplevelConfigure(xdg_toplevel* xdgToplevel, int32_t width, int32_t height, wl_array* states);
        void onToplevelClose(xdg_toplevel* xdgToplevel);

        xdg_toplevel* _xdgToplevel {};
    };
}
