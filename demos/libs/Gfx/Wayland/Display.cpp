#include "Display.h"

#include "Seat.h"
#include <Core/EventLoop.h>
#include <EGL/eglext.h>
#include <cstring>

namespace Gfx::Wayland
{
    const wl_registry_listener Display::_wlRegistryListener = {
        .global = [](void* data, wl_registry* wlRegistry, uint32_t name, const char* interface, uint32_t version)
        {
            auto* display = reinterpret_cast<Display*>(data);
            display->onWlRegistryGlobal(wlRegistry, name, interface, version);
        },
        .global_remove = [](void* data, wl_registry* wlRegistry, uint32_t name)
        {
            auto* display = reinterpret_cast<Display*>(data);
            display->onWlRegistryGlobalRemove(wlRegistry, name);
        }
    };

    const xdg_wm_base_listener Display::_xdgWmBaseListener = {
        .ping = [](void* data, xdg_wm_base* xdgWmBase, uint32_t serial)
        {
            auto* display = reinterpret_cast<Display*>(data);
            display->onXdgWmBasePing(xdgWmBase, serial);
        }
    };

    RefPtr<Display> Display::connect(const char* name)
    {
        wl_display* wlDisplay = wl_display_connect(name);
        if (!wlDisplay)
            return nullptr;
        return adopt(new Display(wlDisplay));
    }

    Display::Display(NonNull<wl_display*> wlDisplay) : _wlDisplay { wlDisplay }
    {
        _wlRegistry = wl_display_get_registry(_wlDisplay);
        wl_registry_add_listener(_wlRegistry, &_wlRegistryListener, this);
        wl_display_roundtrip(_wlDisplay);

        Core::EventLoop::mainLoop()->addSource({
            .fd = wl_display_get_fd(_wlDisplay),
            .events = POLLIN,
            .prepare = [this]()
            {
                wl_display_flush(_wlDisplay);
            },
            .dispatch = [this]()
            {
                wl_display_dispatch(_wlDisplay);
            }
        });

        _eglDisplay = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, _wlDisplay, NULL);

        EGLint major, minor;
        eglInitialize(_eglDisplay, &major, &minor);
    }

    Display::~Display()
    {
        eglMakeCurrent(_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglTerminate(_eglDisplay);

        wl_registry_destroy(_wlRegistry);

        wl_display_disconnect(_wlDisplay);
    }

    void Display::onWlRegistryGlobal(wl_registry* wlRegistry, uint32_t name, const char* interface, uint32_t version)
    {
        if (strcmp(interface, wl_compositor_interface.name) == 0)
        {
            _globals.wlCompositor = reinterpret_cast<wl_compositor*>(wl_registry_bind(wlRegistry, name, &wl_compositor_interface, 6));
        }
        else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        {
            _globals.xdgWmBase = reinterpret_cast<xdg_wm_base*>(wl_registry_bind(wlRegistry, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(_globals.xdgWmBase, &_xdgWmBaseListener, this);
        }
        else if (strcmp(interface, wl_seat_interface.name) == 0)
        {
            auto* wl_seat = reinterpret_cast<struct wl_seat*>(wl_registry_bind(wlRegistry, name, &wl_seat_interface, 7));
            _seat = Seat::create(wl_seat, { this });
        }
    }

    void Display::onWlRegistryGlobalRemove(wl_registry* wlRegistry, uint32_t name)
    {
    }

    void Display::onXdgWmBasePing(xdg_wm_base* xdgWmBase, uint32_t serial)
    {
        xdg_wm_base_pong(xdgWmBase, serial);
    }
}
