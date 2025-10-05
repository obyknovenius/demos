#include "wayland_window.h"

#include "wayland_display.h"
#include <cstring>
#include <gfx/cairo/context.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

namespace gui
{
    const xdg_surface_listener wayland_window::s_xdg_surface_listener = {
        .configure = [](void* data, xdg_surface* xdg_surface, uint32_t serial)
        {
            auto* window = reinterpret_cast<wayland_window*>(data);
            window->on_surface_configure(xdg_surface, serial);
        }
    };

    const wl_buffer_listener wayland_window::s_wl_buffer_listener = {
        .release = [](void* data, wl_buffer* buffer)
        {
            auto* window = reinterpret_cast<wayland_window*>(data);
            window->on_buffer_release(buffer);
        }
    };

    wayland_window::wayland_window(const nonnull_ref_ptr<wayland_display>& display) : m_display { display }
    {
        m_wl_surface = wl_compositor_create_surface(display->get_wl_compositor());
        wl_surface_set_user_data(m_wl_surface, this);

        m_xdg_surface = xdg_wm_base_get_xdg_surface(display->get_xdg_wm_base(), m_wl_surface);
        xdg_surface_add_listener(m_xdg_surface, &s_xdg_surface_listener, this);

        m_xdg_toplevel = xdg_surface_get_toplevel(m_xdg_surface);
        xdg_toplevel_set_title(m_xdg_toplevel, "Hello World");
        wl_surface_commit(m_wl_surface);
    }

    wayland_window::~wayland_window()
    {
        close();
    }

    void wayland_window::close()
    {
        if (m_closed)
            return;
        
        xdg_toplevel_destroy(m_xdg_toplevel);
        xdg_surface_destroy(m_xdg_surface);
        wl_surface_destroy(m_wl_surface);

        m_closed = true;

        window::close();
    }

    void wayland_window::on_surface_configure(xdg_surface* xdg_surface, uint32_t serial)
    {
        xdg_surface_ack_configure(xdg_surface, serial);

        auto display = m_display.strong_ref();
        if (!display)
            return;

        const int stride = m_size.width * 4;
        const int size = stride * m_size.height;

        int fd = memfd_create("shm_pool", 0);
        ftruncate(fd, size);

        uint32_t *pixels = reinterpret_cast<uint32_t*>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (pixels == MAP_FAILED) {
            std::cerr << "mmap failed" << std::endl;
            ::close(fd);
            return;
        }

        wl_shm_pool* shm_pool = wl_shm_create_pool(display->get_wl_shm(), fd, size);
        wl_buffer* wl_buffer = wl_shm_pool_create_buffer(shm_pool, 0, m_size.width, m_size.height, stride, WL_SHM_FORMAT_XRGB8888);
        wl_shm_pool_destroy(shm_pool);
        ::close(fd);

        auto* cairo_surface = cairo_image_surface_create_for_data(
            reinterpret_cast<unsigned char*>(pixels), CAIRO_FORMAT_RGB24,
            m_size.width, m_size.height, stride
        );
        auto* cr = cairo_create(cairo_surface);
        auto context = gfx::cairo::context::create(cr);
        redraw(context);
        cairo_surface_destroy(cairo_surface);

        munmap(pixels, size);
        wl_buffer_add_listener(wl_buffer, &s_wl_buffer_listener, this);

        wl_surface_attach(m_wl_surface, wl_buffer, 0, 0);
        wl_surface_commit(m_wl_surface);
    }

    void wayland_window::on_buffer_release(wl_buffer* buffer)
    {
        wl_buffer_destroy(buffer);
    }
}
