
#include "window.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <optional>
#include <map>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <GLES3/gl3.h>
#include "xdg-shell-client.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "display_server.h"
#include "layer.h"
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

auto Window::create(const gfx::Rect& frame) -> std::shared_ptr<Window>
{
    auto window = std::make_shared<Window>(frame);
    s_window_map[window->m_id] = window;
    return window;
}

Window::Window(const gfx::Rect& frame)
{

    m_id = DisplayServer::shared()->create_window(frame);

    m_decoration_view = std::make_shared<DecorationView>("Hello, Wayland!", frame);
    m_decoration_view->layout();

    m_layer = new Layer(frame);
    m_layer->on_draw = [this](cairo_t* cr) {
        m_decoration_view->draw(cr);
    };
}

Window::~Window()
{
    delete m_layer;
    m_layer = nullptr;
}

}
