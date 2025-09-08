#pragma once

#include "event.h"
#include <core/ref_counted.h>

namespace gui {

class window;

class display : public ref_counted
{
public:
    static auto connect() -> ref_ptr<display>;

    virtual auto create_window() -> ref_ptr<window> = 0;

    auto dispatch_event(const event event) -> void;

protected:
    display() = default;
    virtual ~display() = default;
};

}
