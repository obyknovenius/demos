#pragma once

#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/Weakable.h>
#include <EGL/egl.h>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace Gfx::Wayland
{
    class Seat;

    struct Globals
    {
        wl_compositor* wlCompositor {};
        xdg_wm_base* xdgWmBase {};
    };

    class Display final : public RefCounted, public Weakable
    {
    public:
        static RefPtr<Display> connect(const char* name = nullptr);

        NonNull<wl_display*> wlDisplay() const { return _wlDisplay; }

        Globals globals() const { return _globals; }

        EGLDisplay eglDisplay() const { return _eglDisplay; }

    private:
        static const wl_registry_listener _wlRegistryListener;
        static const xdg_wm_base_listener _xdgWmBaseListener;

        Display(NonNull<wl_display*> wlDisplay);
        ~Display() override;

        void onWlRegistryGlobal(wl_registry* wlRegistry, uint32_t name, const char* interface, uint32_t version);
        void onWlRegistryGlobalRemove(wl_registry* wlRegistry, uint32_t name);

        void onXdgWmBasePing(xdg_wm_base* xdgWmBase, uint32_t serial);

        NonNull<wl_display*> _wlDisplay;

        wl_registry* _wlRegistry {};

        Globals _globals;

        EGLDisplay _eglDisplay { EGL_NO_DISPLAY };

        RefPtr<Seat> _seat;
    };
}
