#pragma once

#include "../Event.h"
#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>
#include <Foundation/WeakPtr.h>
#include <optional>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Seat;
    class Window;

    class Pointer final : public Object
    {
    public:
        Pointer(wl_pointer* wlPointer, StrongPtr<Seat> seat);

    private:
        static const wl_pointer_listener _wlPointerListener;
        
        ~Pointer() override;

        void didEnterWindow(uint32_t serial, StrongPtr<Window> window, double x, double y);
        void didLeaveWindow(uint32_t serial, StrongPtr<Window> window);
        void didMove(uint32_t time, double x, double y);

        void sendEvent();

        wl_pointer* _wlPointer = nullptr;

        WeakPtr<Seat> _seat = nullptr;

        WeakPtr<Window> _window = nullptr;

        std::optional<Event> _event = std::nullopt;
    };
}
