#pragma once

#include <initializer_list>

namespace Core
{
    template<typename T>
    class OptionSet {
    public:
        using UT = std::underlying_type_t<T>;

        OptionSet() = default;

        OptionSet(T option) : _value { static_cast<UT>(option) }
        {
        }

        OptionSet(std::initializer_list<T> options)
        {
            for (auto& option : options)
                _value |= static_cast<UT>(option);
        }

        OptionSet& operator|=(T option)
        {
            _value |= static_cast<UT>(option);
            return *this;
        }

        OptionSet operator&(T option) const
        {
            return { _value & static_cast<UT>(option) };
        }

        bool operator==(const OptionSet& other) const
        {
            return _value == other._value;
        }

        operator bool() const
        {
            return _value;
        }

    private:
        OptionSet(UT value) : _value { value }
        {
        }

        UT _value { 0 };
    };

    template<typename T>
    requires std::is_enum_v<T>
    OptionSet<T> operator|(T lhs, T rhs)
    {
        return { lhs, rhs };
    }
}

using Core::OptionSet;
using Core::operator|;
