#pragma once

#include <core/ref_counted.h>
#include <functional>

namespace gui {

struct event;

class window : public ref_counted
{
public:
    auto dispatch_event(const event event) -> void;

    virtual auto close() -> void;

    std::function<void()> on_close;

protected:
    window() = default;
    virtual ~window() = default;
};

}
