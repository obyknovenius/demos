#pragma once

#include "../Event.h"
#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/WeakPtr.h>
#include <optional>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Seat;
    class Window;

    class Pointer final : public RefCounted
    {
    public:
        static NonNull<RefPtr<Pointer>> create(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat);

    private:
        static const wl_pointer_listener _wlPointerListener;

        Pointer(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat);
        ~Pointer();

        void didEnterWindow(uint32_t serial, NonNull<RefPtr<Window>> window, double x, double y);
        void didLeaveWindow(uint32_t serial, NonNull<RefPtr<Window>> window);
        void didMove(uint32_t time, double x, double y);

        void sendEvent();

        NonNull<wl_pointer*> _wlPointer;

        WeakPtr<Seat> _seat {};

        WeakPtr<Window> _window {};

        std::optional<Event> _event {};
    };
}
