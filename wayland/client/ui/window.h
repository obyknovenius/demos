#pragma once

#include <EGL/egl.h>
#include <cairo/cairo.h>
#include <memory>

namespace gfx {
    struct Rect;
}

namespace ui {

class Display;
class Layer;

class Window final : public std::enable_shared_from_this<Window>  {
public:
    class DecorationView;

    static auto create(const gfx::Rect& frame) -> std::shared_ptr<Window>;

    static auto with_id(int id) -> std::shared_ptr<Window>;

    Window(const gfx::Rect& frame);
    ~Window();

    auto layer() const -> Layer* { return m_layer; }

    auto configure(struct xdg_surface* xdg_surface, uint32_t serial) -> void;
    auto frame_done(struct wl_callback* callback, uint32_t time) -> void;

private:
    auto draw() -> void;

    auto draw(cairo_t* cr) -> void;
    auto draw_titlebar(cairo_t* cr) -> void;
    auto draw_placeholder(cairo_t* cr, const gfx::Rect& rect, float dx) -> void;

    int m_id;

    bool m_animate { false };

    int m_width {};
    int m_height {};

    struct wl_surface* m_wl_surface {};
    struct xdg_surface* m_xdg_surface {};
    struct xdg_toplevel* m_xdg_toplevel {};

    struct wl_egl_window* m_wl_egl_window {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY};
    EGLSurface m_egl_surface { EGL_NO_SURFACE };
    EGLContext m_egl_context { EGL_NO_CONTEXT };

    unsigned int m_program;

    Layer* m_layer {};

    std::shared_ptr<DecorationView> m_decoration_view {};
};

}
