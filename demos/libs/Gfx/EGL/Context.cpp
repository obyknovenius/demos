#include "Context.h"

#include "../Wayland/Display.h"
#include "../Wayland/Surface.h"
#include <GLES3/gl3.h>

namespace Gfx::EGL
{
    NonNull<RefPtr<Context>> Context::create(NonNull<RefPtr<Wayland::Surface>> surface)
    {
        return adopt(new Context(surface));
    }

    Context::Context(NonNull<RefPtr<Wayland::Surface>> surface) : _surface { surface }
    {
        EGLint attributes[] = {
            EGL_CONTEXT_MAJOR_VERSION, 3,
            EGL_NONE
        };

        _eglContext = eglCreateContext(surface->display()->eglDisplay(), surface->eglConfig(), EGL_NO_CONTEXT, attributes);
    }

    Context::~Context()
    {
        eglDestroyContext(_surface->display()->eglDisplay(), _eglContext);
    }

    void Context::beginFrame()
    {
        eglMakeCurrent(_surface->display()->eglDisplay(), _surface->eglSurface(), _surface->eglSurface(), _eglContext);
        eglSwapInterval(_surface->display()->eglDisplay(), 0);

        glViewport(0, 0, _surface->size().width, _surface->size().height);
    }

    void Context::endFrame()
    {
        eglSwapBuffers(_surface->display()->eglDisplay(), _surface->eglSurface());
    }
}
