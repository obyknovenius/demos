#include "window.h"

#include "event.h"
#include "window_decoration_view.h"
#include "window_title_bar.h"
#include "wayland/window.h"
#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/rect.h>

namespace gui
{
    NonnullRefPtr<window> window::make()
    {
        return wayland::window::make();
    }

    window::window(gfx::size const& size) :
        m_size { size },
        m_decoration_view { decoration_view::make(*this) }
    {
    }

    window::~window() = default;

    bool window::should_start_move(const gfx::point& pointer_position) const
    {
        if (m_decoration_view->get_title_bar()->get_frame().contains(pointer_position))
            return true;
        return false;
    }

    void window::dispatch_event(std::unique_ptr<const event> event)
    {
        switch (event->type)
        {
            case event::type::button_pressed:
            {
                auto view = m_decoration_view->hit_test(event->position);
                view->on_button_pressed(std::move(event));
                break;
            }

            default:
                break;
        }
    }

    void window::close()
    {
        if (on_close)
            on_close();
    }

    void window::layout()
    {
        m_decoration_view->set_frame({ 0, 0, m_size.width, m_size.height });
        m_decoration_view->layout();
    }

    void window::redraw(NonnullRefPtr<gfx::context> context)
    {
        context->fill_rect({0, 0, m_size.width, m_size.height}, gfx::color::white);
        m_decoration_view->redraw(context);
    }
}
