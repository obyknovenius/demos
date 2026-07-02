#pragma once

#include "../Display.h"
#include "Seat.h"
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
        static StrongPtr<Display> connect(std::optional<std::string_view> name = std::nullopt);

        wl_compositor* wlCompositor() const { return _wlCompositor; }
        xdg_wm_base* xdgWmBase() const { return _xdgWmBase; }

        EGLDisplay eglDisplay() const { return _eglDisplay; }

    private:
        static const wl_registry_listener _wlRegistryListener;
        static const xdg_wm_base_listener _xdgWmBaseListener;

        Display(wl_display* wlDisplay);
        ~Display() override;

        void addGlobal(uint32_t name, std::string_view interface, uint32_t version);
        void ping(uint32_t serial);

        wl_display* _wlDisplay = nullptr;

        wl_registry* _wlRegistry = nullptr;
        wl_compositor* _wlCompositor = nullptr;
        xdg_wm_base* _xdgWmBase = nullptr;

        EGLDisplay _eglDisplay = EGL_NO_DISPLAY;

        StrongPtr<Seat> _seat = nullptr;
    };
}
