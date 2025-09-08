#pragma once

#include <core/ref_ptr.h>

namespace core {

class ref_counted
{
public:
    void ref()
    {
        ++m_ref_count;
    }

    void unref()
    {
        if (--m_ref_count == 0)
            delete this;
    }

protected:
    ref_counted() = default;
    virtual ~ref_counted() = default;

private:
    int m_ref_count { 0 };
};

template<typename T, class... Args>
ref_ptr<T> make_ref_counted(Args&&... args)
{
    return ref_ptr<T>(new T(std::forward<Args>(args)...));
}

}

using core::ref_counted;
using core::make_ref_counted;
