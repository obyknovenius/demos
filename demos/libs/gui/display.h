#pragma once

#include <core/weakable.h>
#include <memory>

namespace gui {

class window;
struct event;

class display : public weakable
{
public:
    static auto connect() -> ref_ptr<display>;

    virtual auto create_window() -> nonnull_ref_ptr<window> = 0;

    auto dispatch_event(std::unique_ptr<const event> event) -> void;

protected:
    display() = default;
    virtual ~display() = default;
};

}
