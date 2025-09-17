#pragma once

#include "event.h"
#include <core/weakable.h>

namespace gui {

class window;

class display : public weakable
{
public:
    static auto connect() -> ref_ptr<display>;

    virtual auto create_window() -> nonnull_ref_ptr<window> = 0;

    auto dispatch_event(const event event) -> void;

protected:
    display() = default;
    virtual ~display() = default;
};

}
