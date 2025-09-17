#pragma once

#include <core/nonnull_ref_ptr.h>
#include <core/ref_counted.h>

namespace core {

class ref_counted
{
    template<typename T>
    friend class ref_ptr;

    template<typename T>
    friend class nonnull_ref_ptr;

protected:
    ref_counted() = default;
    virtual ~ref_counted() = default;

private:
    void ref()
    {
        ++m_ref_count;
    }

    void unref()
    {
        if (--m_ref_count == 0)
            delete this;
    }

    int m_ref_count { 0 };
};

template<typename T, class... Args>
nonnull_ref_ptr<T> make_ref_counted(Args&&... args)
{
    return nonnull_ref_ptr<T>(*new T(std::forward<Args>(args)...));
}

}

using core::ref_counted;
using core::make_ref_counted;
