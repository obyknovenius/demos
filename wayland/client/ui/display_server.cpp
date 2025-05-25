#include "display_server.h"

#include "wayland/display_server.h"

namespace ui {

auto DisplayServer::shared() -> std::shared_ptr<DisplayServer>
{
    static std::shared_ptr<DisplayServer> s_shared;
    static std::once_flag s_once_flag;
    std::call_once(s_once_flag, []
    {
        s_shared = std::shared_ptr<DisplayServer>(create());
    });
    return s_shared;
}

DisplayServer* DisplayServer::create()
{
    return new wayland::DisplayServer();
}

}
