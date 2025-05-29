
#include "window.h"

#include <map>
#include <memory>

#include "layer.h"
#include "display_server.h"
#include "window_decoration_view.h"

namespace ui {

static std::map<int, std::shared_ptr<Window>> s_window_map {};

auto Window::with_id(int id) -> std::shared_ptr<Window>
{
    auto it = s_window_map.find(id);
    if (it != s_window_map.end()) {
        return it->second;
    }
    return nullptr;
}

auto Window::create(const gfx::Size& size) -> std::shared_ptr<Window>
{
    auto window = std::make_shared<Window>(size);
    s_window_map[window->m_id] = window;
    return window;
}

Window::Window(const gfx::Size& size)
{
    m_id = DisplayServer::shared()->create_window(size);

    m_decoration_view = std::make_shared<DecorationView>("Hello, Wayland!", gfx::Rect { 0, 0, size.width, size.height });
    m_decoration_view->layout();

    m_layer = new Layer({ 0, 0, size.width, size.height });
    m_layer->on_draw = [this](cairo_t* cr) {
        m_decoration_view->draw(cr);
    };
}

Window::~Window()
{
    delete m_layer;
    m_layer = nullptr;
}

auto Window::draw() -> void
{
    DisplayServer::shared()->draw_window(m_id);
}

}
