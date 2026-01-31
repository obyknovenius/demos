#pragma once

#include "../Display.h"
#include "Seat.h"
#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/Weakable.h>
#include <EGL/egl.h>
#include <optional>
#include <string_view>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace Platform::Wayland
{
    class Display : public Platform::Display
    {
    public:
        static RefPtr<Display> connect(std::optional<std::string_view> name = std::nullopt);

        NonNull<wl_compositor*> wlCompositor() const { return _wlCompositor; }
        NonNull<xdg_wm_base*> xdgWmBase() const { return _xdgWmBase; }

        EGLDisplay eglDisplay() const { return _eglDisplay; }

    private:
        static const wl_registry_listener _wlRegistryListener;
        static const xdg_wm_base_listener _xdgWmBaseListener;

        Display(NonNull<wl_display*> wlDisplay);
        ~Display() override;

        void addGlobal(uint32_t name, std::string_view interface, uint32_t version);
        void ping(uint32_t serial);

        NonNull<wl_display*> _wlDisplay;

        wl_registry* _wlRegistry {};
        wl_compositor* _wlCompositor {};
        xdg_wm_base* _xdgWmBase {};

        EGLDisplay _eglDisplay { EGL_NO_DISPLAY };

        RefPtr<Seat> _seat {};
    };
}
