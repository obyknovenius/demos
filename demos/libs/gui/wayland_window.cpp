#include "wayland_window.h"

#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

namespace gui {

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

wayland_window::wayland_window(wayland_display* display) : m_display(display)
{
    m_wl_surface = wl_compositor_create_surface(m_display->m_wl_compositor);
    wl_surface_set_user_data(m_wl_surface, this);

    m_xdg_surface = xdg_wm_base_get_xdg_surface(m_display->m_xdg_wm_base, m_wl_surface);
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
    
    xdg_toplevel_destroy(m_xdg_toplevel);
    xdg_surface_destroy(m_xdg_surface);
    wl_surface_destroy(m_wl_surface);

    m_closed = true;

    window::close();
}

auto wayland_window::on_surface_configure(xdg_surface* xdg_surface, uint32_t serial) -> void
{
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

    wl_shm_pool* shm_pool = wl_shm_create_pool(m_display->m_wl_shm, fd, size);
    wl_buffer* wl_buffer = wl_shm_pool_create_buffer(shm_pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(shm_pool);
    ::close(fd);

    memset(pixels, 0, size);

    munmap(pixels, size);
    wl_buffer_add_listener(wl_buffer, &s_wl_buffer_listener, this);

    wl_surface_attach(m_wl_surface, wl_buffer, 0, 0);
    wl_surface_commit(m_wl_surface);
}

auto wayland_window::on_buffer_release(wl_buffer* buffer) -> void
{
    wl_buffer_destroy(buffer);
}

}
