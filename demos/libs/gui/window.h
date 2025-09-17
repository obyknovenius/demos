#pragma once

#include <core/ref_counted.h>
#include <functional>
#include <memory>

namespace gui {

struct event;

class window : public ref_counted
{
public:
    auto dispatch_event(std::unique_ptr<const event> event) -> void;

    virtual auto close() -> void;

    std::function<void()> on_close;

protected:
    window() = default;
    virtual ~window() = default;
};

}
