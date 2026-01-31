#pragma once

#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/WeakPtr.h>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Seat;

    class Pointer final : public RefCounted
    {
    public:
        static NonNull<RefPtr<Pointer>> create(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat);

    private:
        static const wl_pointer_listener _wlPointerListener;

        Pointer(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat);
        ~Pointer();

        NonNull<wl_pointer*> _wlPointer;

        WeakPtr<Seat> _seat {};
    };
}
