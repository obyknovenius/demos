#pragma once

#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <EGL/egl.h>

namespace Gfx::Wayland
{
    class Surface;
}

namespace Gfx::EGL
{
    class Context final : public RefCounted
    {
    public:
        static NonNull<RefPtr<Context>> create(NonNull<RefPtr<Wayland::Surface>> surface);

        void beginFrame();
        void endFrame();

    private:
        Context(NonNull<RefPtr<Wayland::Surface>> surface);
        ~Context() override;

        NonNull<RefPtr<Wayland::Surface>> _surface;

        EGLContext _eglContext { EGL_NO_CONTEXT };
    };
}
