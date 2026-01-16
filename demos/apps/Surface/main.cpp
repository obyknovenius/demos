#include <Core/EventLoop.h>
#include <GUI/Wayland/Display.h>
#include <GUI/Wayland/Toplevel.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <wayland-egl.h>

int main(int argc, char** argv)
{
    auto display = GUI::Wayland::Display::defaultDisplay();

    auto eglDisplay = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, display->wlDisplay(), NULL);

    EGLint major, minor;
    eglInitialize(eglDisplay, &major, &minor);

    auto toplevel = GUI::Wayland::Toplevel::make(display);

    auto* eglWindow = wl_egl_window_create(toplevel->wlSurface(), toplevel->size().width, toplevel->size().height);

    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLConfig eglConfig;
    EGLint numConfig;

    eglChooseConfig(eglDisplay, configAttribs, &eglConfig, 1, &numConfig);

    auto eglSurface = eglCreatePlatformWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

    EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_NONE
    };
    auto eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttribs);

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

    glViewport(0, 0, toplevel->size().width, toplevel->size().height);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(eglDisplay, eglSurface);

    auto mainLoop = Core::EventLoop::mainLoop();
    mainLoop->run();
    return 0;
}
