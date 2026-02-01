#include "Display.h"

#include <Core/EventLoop.h>
#include <EGL/eglext.h>

namespace Platform::Wayland
{
    RefPtr<Display> Display::connect(std::optional<std::string_view> name)
    {
        wl_display* wlDisplay = wl_display_connect(name ? name->data() : nullptr);
        if (!wlDisplay)
            return nullptr;
        return adopt(new Display(wlDisplay));
    }

    const wl_registry_listener Display::_wlRegistryListener = {
        .global = [](void* data, wl_registry* wlRegistry, uint32_t name, const char* interface, uint32_t version)
        {
            auto* display = reinterpret_cast<Display*>(data);
            display->addGlobal(name, interface, version);
        },
        .global_remove = nullptr
    };

    const xdg_wm_base_listener Display::_xdgWmBaseListener = {
        .ping = [](void* data, xdg_wm_base* xdgWmBase, uint32_t serial)
        {
            auto* display = reinterpret_cast<Display*>(data);
            display->ping(serial);
        }
    };

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

        xdg_wm_base_destroy(_xdgWmBase);
        wl_compositor_destroy(_wlCompositor);
        wl_registry_destroy(_wlRegistry);

        wl_display_disconnect(_wlDisplay);
    }

    void Display::addGlobal(uint32_t name, std::string_view interface, uint32_t version)
    {
        if (interface == wl_compositor_interface.name)
        {
            _wlCompositor = reinterpret_cast<wl_compositor*>(wl_registry_bind(_wlRegistry, name, &wl_compositor_interface, 6));
        }
        else if (interface == xdg_wm_base_interface.name)
        {
            _xdgWmBase = reinterpret_cast<xdg_wm_base*>(wl_registry_bind(_wlRegistry, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(_xdgWmBase, &_xdgWmBaseListener, this);
        }
        else if (interface == wl_seat_interface.name)
        {
            wl_seat* wlSeat = reinterpret_cast<wl_seat*>(wl_registry_bind(_wlRegistry, name, &wl_seat_interface, 1));
            _seat = Seat::create(wlSeat, this);
        }
    }

    void Display::ping(uint32_t serial)
    {
        xdg_wm_base_pong(_xdgWmBase, serial);
    }
}
