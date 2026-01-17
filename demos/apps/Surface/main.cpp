#include <Core/EventLoop.h>
#include <GUI/Wayland/Display.h>
#include <GUI/Wayland/Toplevel.h>
#include <GLES3/gl3.h>

int main(int argc, char** argv)
{
    auto display = GUI::Wayland::Display::defaultDisplay();
    auto toplevel = GUI::Wayland::Toplevel::make(display);

    EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_NONE
    };
    auto eglContext = eglCreateContext(display->eglDisplay(), display->eglConfig(), EGL_NO_CONTEXT, contextAttribs);

    eglMakeCurrent(display->eglDisplay(), toplevel->eglSurface(), toplevel->eglSurface(), eglContext);

    glViewport(0, 0, toplevel->size().width, toplevel->size().height);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(display->eglDisplay(), toplevel->eglSurface());

    auto mainLoop = Core::EventLoop::mainLoop();
    mainLoop->run();
    return 0;
}
