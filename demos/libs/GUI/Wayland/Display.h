#pragma once

#include "Forward.h"
#include "Seat.h"
#include "../Forward.h"
#include <Core/NonNull.h>
#include <Core/Weakable.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <memory>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <cursor-shape-v1-client-protocol.h>

namespace GUI::Wayland
{
    class Display final : public RefCounted, public Weakable
    {
    public:
        static RefPtr<Display> defaultDisplay();

        NonNull<wl_display*> wlDisplay() { return _wlDisplay; }

        EGLDisplay eglDisplay() { return _eglDisplay; }
        EGLConfig eglConfig() { return _eglConfig; }

        wl_compositor* wlCompositor() { return _wlCompositor; }
        xdg_wm_base* xdgWmBase() { return _xdgWmBase; }
        wl_shm* wlShm() { return _wlShm; }
        wp_cursor_shape_manager_v1* wpCursorShapeManagerV1() { return _wpCursorShapeManagerV1; }

        RefPtr<Seat> seat() { return _seat; }

        void dispatchEvent(std::unique_ptr<const Event> event);

    private:
        static RefPtr<Display> _defaultDisplay;

        Display(NonNull<wl_display*> wlDisplay);
        ~Display();

        static const wl_registry_listener _wlRegistryListener;
        static const xdg_wm_base_listener _xdgWmBaseListener;

        void onRegistryGlobal(wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
        void onWmPing(xdg_wm_base* xdgWmBase, uint32_t serial);

        NonNull<wl_display*> _wlDisplay;

        EGLDisplay _eglDisplay { EGL_NO_DISPLAY };
        EGLConfig _eglConfig { EGL_NO_CONFIG_KHR };

        wl_registry* _wlRegistry {};
        wl_compositor* _wlCompositor {};
        xdg_wm_base* _xdgWmBase {};
        wl_shm* _wlShm {};
        wp_cursor_shape_manager_v1* _wpCursorShapeManagerV1 {};

        RefPtr<Seat> _seat;
    };
}
