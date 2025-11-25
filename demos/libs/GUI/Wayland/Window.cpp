#include "Window.h"

#include "Display.h"
#include "Pointer.h"
#include "Seat.h"
#include <Gfx/Cairo/Context.h>
#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

namespace GUI::Wayland
{
    const xdg_surface_listener Window::_xdgSurfaceListener = {
        .configure = [](void* data, xdg_surface* xdgSurface, uint32_t serial)
        {
            auto* window = reinterpret_cast<Window*>(data);
            window->onSurfaceConfigure(xdgSurface, serial);
        }
    };

    const xdg_toplevel_listener Window::_xdgToplevelListener = {
        .configure = [](void* data, xdg_toplevel* xdgToplevel, int32_t width, int32_t height, wl_array* states)
        {
            auto* window = reinterpret_cast<Window*>(data);
            window->onToplevelConfigure(xdgToplevel, width, height, states);
        },
        .close = [](void* data, xdg_toplevel* xdgToplevel)
        {
            auto* window = reinterpret_cast<Window*>(data);
            window->onToplevelClose(xdgToplevel);
        },
        .configure_bounds = nullptr,
        .wm_capabilities = nullptr
    };

    const wl_buffer_listener Window::_wlBufferListener = {
        .release = [](void* data, wl_buffer* buffer)
        {
            auto* window = reinterpret_cast<Window*>(data);
            window->onBufferRelease(buffer);
        }
    };

    NonnullRefPtr<Window> Window::make()
    {
        auto display = Display::defaultDisplay();
        return adopt(*new Window(*display));
    }

    Window::Window(const NonnullRefPtr<Display>& display) : _display { display }
    {
        _wlSurface = wl_compositor_create_surface(display->wlCompositor());
        wl_surface_set_user_data(_wlSurface, this);

        _xdgSurface = xdg_wm_base_get_xdg_surface(display->xdgWmBase(), _wlSurface);
        xdg_surface_add_listener(_xdgSurface, &_xdgSurfaceListener, this);

        _xdgToplevel = xdg_surface_get_toplevel(_xdgSurface);
        xdg_toplevel_set_title(_xdgToplevel, "Hello World");
        xdg_toplevel_add_listener(_xdgToplevel, &_xdgToplevelListener, this);
        wl_surface_commit(_wlSurface);
    }

    Window::~Window()
    {
        close();
    }

    void Window::close()
    {
        if (_closed)
            return;

        xdg_toplevel_destroy(_xdgToplevel);
        xdg_surface_destroy(_xdgSurface);
        wl_surface_destroy(_wlSurface);

        _closed = true;

        GUI::Window::close();
    }

    void Window::setCursor(Cursor cursor)
    {
        GUI::Window::setCursor(cursor);

        if (auto display = _display.strong())
            if (auto seat = display->seat())
                if (auto pointer = seat->pointer())
                    pointer->setCursor(cursor);
    }

    void Window::onSurfaceConfigure(xdg_surface* xdgSurface, uint32_t serial)
    {
        xdg_surface_ack_configure(xdgSurface, serial);

        auto display = _display.strong();
        if (!display)
            return;

        if (_wlBuffer)
        {
            wl_buffer_destroy(_wlBuffer);
            _wlBuffer = nullptr;
        }

        const int stride = _size.width * 4;
        const int size = stride * _size.height;

        int fd = memfd_create("shm_pool", 0);
        ftruncate(fd, size);

        void* pixels = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (pixels == MAP_FAILED) {
            std::cerr << "mmap failed" << std::endl;
            ::close(fd);
            return;
        }

        wl_shm_pool* shmPool = wl_shm_create_pool(display->wlShm(), fd, size);
        wl_buffer* wlBuffer = wl_shm_pool_create_buffer(shmPool, 0, _size.width, _size.height, stride, WL_SHM_FORMAT_XRGB8888);
        wl_buffer_add_listener(wlBuffer, &_wlBufferListener, this);
        wl_shm_pool_destroy(shmPool);

        layout();

        auto* cairoSurface = cairo_image_surface_create_for_data(
            reinterpret_cast<unsigned char*>(pixels), CAIRO_FORMAT_RGB24,
            _size.width, _size.height, stride
        );
        auto* cr = cairo_create(cairoSurface);
        auto context = Gfx::Cairo::Context::make(cr);
        redraw(context);
        cairo_surface_destroy(cairoSurface);

        wl_surface_attach(_wlSurface, wlBuffer, 0, 0);
        wl_surface_commit(_wlSurface);

        munmap(pixels, size);
        ::close(fd);
    }

    void Window::onToplevelConfigure(xdg_toplevel* xdgToplevel, int32_t width, int32_t height, wl_array* states)
    {
        if (width == 0 || height == 0)
            return;

        _size = { width, height };
    }

    void Window::onToplevelClose(xdg_toplevel* xdgToplevel)
    {
        close();
    }

    void Window::onBufferRelease(wl_buffer* buffer)
    {
        wl_buffer_destroy(buffer);
    }
}
