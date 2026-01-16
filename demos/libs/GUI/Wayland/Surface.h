#pragma once

#include "Forward.h"
#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/WeakPtr.h>
#include <Gfx/Size.h>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace GUI::Wayland
{
    class Surface : public RefCounted
    {
    public:
        static NonNull<RefPtr<Surface>> make(RefPtr<Display> display) { return adopt(new Surface(display)); }

        NonNull<wl_surface*> wlSurface() const { return _wlSurface; }

        Gfx::Size size() const { return _size; }

    protected:
        Surface(NonNull<RefPtr<Display>> display);
        ~Surface() override;

        void onSurfaceEnter(wl_surface* wlSurface, wl_output* output);
        void onSurfaceLeave(wl_surface* wlSurface, wl_output* output);
        void onSurfacePreferredBufferScale(wl_surface* wlSurface, int32_t scale);
        void onSurfacePreferredBufferTransform(wl_surface* wlSurface, uint32_t transform);

        void onSurfaceConfigure(xdg_surface* xdgSurface, uint32_t serial);

        WeakPtr<Display> _display;

        Gfx::Size _size { 800, 600 };

        wl_surface* _wlSurface {};
        xdg_surface* _xdgSurface {};

    private:
        static const wl_surface_listener _wlSurfaceListener;
        static const xdg_surface_listener _xdgSurfaceListener;
    };
}
