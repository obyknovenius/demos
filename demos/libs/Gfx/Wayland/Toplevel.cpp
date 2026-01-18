#include "Toplevel.h"

#include "Display.h"

namespace Gfx::Wayland
{
    const xdg_toplevel_listener Toplevel::_xdgToplevelListener = {
        .configure = [](void* data, xdg_toplevel* xdgToplevel, int32_t width, int32_t height, wl_array* states)
        {
            auto* toplevel = reinterpret_cast<Toplevel*>(data);
            toplevel->onToplevelConfigure(xdgToplevel, width, height, states);
        },
        .close = [](void* data, xdg_toplevel* xdgToplevel)
        {
            auto* toplevel = reinterpret_cast<Toplevel*>(data);
            toplevel->onToplevelClose(xdgToplevel);
        },
        .configure_bounds = nullptr,
        .wm_capabilities = nullptr
    };

    NonNull<RefPtr<Toplevel>> Toplevel::create(NonNull<RefPtr<Display>> display)
    {
        return adopt(new Toplevel(display));
    }

    Toplevel::Toplevel(NonNull<RefPtr<Display>> display) : Surface(display)
    {
        _xdgToplevel = xdg_surface_get_toplevel(_xdgSurface);

        wl_surface_commit(_wlSurface);
    }

    Toplevel::~Toplevel()
    {
        xdg_toplevel_destroy(_xdgToplevel);
    }

    void Toplevel::onToplevelConfigure(xdg_toplevel* xdgToplevel, int32_t width, int32_t height, wl_array* states)
    {
    }

    void Toplevel::onToplevelClose(xdg_toplevel* xdgToplevel)
    {
    }
}
