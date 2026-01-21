#pragma once

#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/WeakPtr.h>
#include <wayland-client.h>

namespace Gfx::Wayland
{
    class Seat;
    class Surface;

    class Pointer final : public RefCounted
    {
    public:
        static NonNull<RefPtr<Pointer>> create(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat)
        {
            return adopt(new Pointer(wlPointer, seat));
        }

        NonNull<wl_pointer*> wlPointer() { return _wlPointer; }

        WeakPtr<Seat> seat() { return _seat; }

    private:
        static const wl_pointer_listener _wlPointerListener;

        Pointer(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat);
        ~Pointer();

        void onWlPointerEnter(wl_pointer* wlPointer, uint32_t serial, wl_surface* wlSurface, wl_fixed_t x, wl_fixed_t y);
        void onWlPointerLeave(wl_pointer* wlPointer, uint32_t serial, wl_surface* wlSurface);
        void onWlPointerMotion(wl_pointer* wlPointer, uint32_t time, wl_fixed_t x, wl_fixed_t y);
        void onWlPointerButton(wl_pointer* wlPointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
        void onWlPointerAxis(wl_pointer* wlPointer, uint32_t time, uint32_t axis, wl_fixed_t value);
        void onWlPointerFrame(wl_pointer* wlPointer);
        void onWlPointerAxisSource(wl_pointer* wlPointer, uint32_t axisSource);
        void onWlPointerAxisStop(wl_pointer* wlPointer, uint32_t time, uint32_t axisStop);
        void onWlPointerAxisDiscrete(wl_pointer* wlPointer, uint32_t axis, int32_t discrete);

        NonNull<wl_pointer*> _wlPointer;

        WeakPtr<Seat> _seat;

        RefPtr<Surface> _surfaceUnderPointer;
    };
}
