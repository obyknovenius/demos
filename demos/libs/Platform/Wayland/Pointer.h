#pragma once

#include "../Event.h"
#include <Foundation/NonNull.h>
#include <Foundation/NonNullRefPtr.h>
#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>
#include <Foundation/WeakPtr.h>
#include <optional>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Seat;
    class Window;

    class Pointer final : public RefCounted
    {
    public:
        static NonNullRefPtr<Pointer> create(NonNull<wl_pointer*> wlPointer, NonNullRefPtr<Seat> seat);

    private:
        static const wl_pointer_listener _wlPointerListener;

        Pointer(NonNull<wl_pointer*> wlPointer, NonNullRefPtr<Seat> seat);
        ~Pointer();

        void didEnterWindow(uint32_t serial, NonNullRefPtr<Window> window, double x, double y);
        void didLeaveWindow(uint32_t serial, NonNullRefPtr<Window> window);
        void didMove(uint32_t time, double x, double y);

        void sendEvent();

        NonNull<wl_pointer*> _wlPointer;

        WeakPtr<Seat> _seat {};

        WeakPtr<Window> _window {};

        std::optional<Event> _event {};
    };
}
