#pragma once

#include <core/weakable.h>
#include <memory>

namespace gui
{
    class window;
    struct event;

    class display : public weakable
    {
    public:
        static ref_ptr<display> connect();

        virtual nonnull_ref_ptr<window> create_window() = 0;

        void dispatch_event(std::unique_ptr<const event> event);

    protected:
        display() = default;
        virtual ~display() = default;
    };
}
