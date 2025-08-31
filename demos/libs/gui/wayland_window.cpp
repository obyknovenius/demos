#include "wayland_window.h"

#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

namespace gui {

const struct xdg_surface_listener wayland_window::s_xdg_surface_listener = {
    .configure = [](void* data, struct xdg_surface* xdg_surface, uint32_t serial)
    {
        auto* window = reinterpret_cast<wayland_window*>(data);

        xdg_surface_ack_configure(xdg_surface, serial);

        const int width = 640, height = 480;
        const int stride = width * 4;
        const int size = stride * height;

        int fd = memfd_create("shm_pool", 0);
        ftruncate(fd, size);

        uint32_t *pixels = reinterpret_cast<uint32_t*>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (pixels == MAP_FAILED) {
            std::cerr << "mmap failed" << std::endl;
            ::close(fd);
            return;
        }

        struct wl_shm_pool* shm_pool = wl_shm_create_pool(window->m_display->wl_shm(), fd, size);
        struct wl_buffer* wl_buffer = wl_shm_pool_create_buffer(shm_pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
        wl_shm_pool_destroy(shm_pool);
        ::close(fd);

        memset(pixels, 0, size);

        munmap(pixels, size);
        wl_buffer_add_listener(wl_buffer, &s_wl_buffer_listener, window);

        wl_surface_attach(window->m_wl_surface, wl_buffer, 0, 0);
        wl_surface_commit(window->m_wl_surface);
    }
};

const struct wl_buffer_listener wayland_window::s_wl_buffer_listener = {
    .release = [](void* data, struct wl_buffer* buffer)
    {
        wl_buffer_destroy(buffer);
    }
};

wayland_window::wayland_window(wayland_display* display) : m_display(display)
{
    m_wl_surface = wl_compositor_create_surface(m_display->wl_compositor());
    wl_surface_set_user_data(m_wl_surface, this);

    m_xdg_surface = xdg_wm_base_get_xdg_surface(m_display->xdg_wm_base(), m_wl_surface);
    xdg_surface_add_listener(m_xdg_surface, &s_xdg_surface_listener, this);

    m_xdg_toplevel = xdg_surface_get_toplevel(m_xdg_surface);
    xdg_toplevel_set_title(m_xdg_toplevel, "Hello World");
    wl_surface_commit(m_wl_surface);
}

wayland_window::~wayland_window()
{
    close();
}

auto wayland_window::close() -> void
{
    if (m_closed)
        return;
    
    wl_surface_destroy(m_wl_surface);

    m_closed = true;

    window::close();
}

}
