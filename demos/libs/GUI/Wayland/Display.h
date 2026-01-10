#pragma once

#include <Core/NonNull.h>
#include <Core/Weakable.h>
#include <memory>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <cursor-shape-v1-client-protocol.h>

namespace GUI
{
    struct Event;
}

namespace GUI::Wayland
{
    class Seat;

    class Display final : public Weakable
    {
    public:
        static RefPtr<Display> defaultDisplay();

        void dispatchEvent(std::unique_ptr<const Event> event);

        wl_compositor* wlCompositor();
        xdg_wm_base* xdgWmBase();
        wl_shm* wlShm();
        wp_cursor_shape_manager_v1* wpCursorShapeManagerV1();

        RefPtr<Seat> seat();

    private:
        static RefPtr<Display> _defaultDisplay;

        Display(NonNull<wl_display*> wlDisplay);
        ~Display();

        static const wl_registry_listener _wlRegistryListener;
        static const xdg_wm_base_listener _xdgWmBaseListener;

        void onRegistryGlobal(wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
        void onWmPing(xdg_wm_base* xdgWmBase, uint32_t serial);

        NonNull<wl_display*> _wlDisplay;
        wl_registry* _wlRegistry;
        wl_compositor* _wlCompositor {};
        xdg_wm_base* _xdgWmBase {};
        wl_shm* _wlShm {};
        wp_cursor_shape_manager_v1* _wpCursorShapeManagerV1 {};

        RefPtr<Seat> _seat;
    };

    inline wl_compositor* Display::wlCompositor()
    {
        return _wlCompositor;
    }

    inline xdg_wm_base* Display::xdgWmBase()
    {
        return _xdgWmBase;
    }

    inline wl_shm* Display::wlShm()
    {
        return _wlShm;
    }

    inline wp_cursor_shape_manager_v1* Display::wpCursorShapeManagerV1()
    {
        return _wpCursorShapeManagerV1;
    }

    inline RefPtr<Seat> Display::seat()
    {
        return _seat;
    }
}
