#pragma once

#include <initializer_list>
#include <type_traits>

namespace Core
{
    template<typename T>
    class OptionSet final {
    public:
        using UT = std::underlying_type_t<T>;

        OptionSet() = default;

        OptionSet(T option) :
            _value { static_cast<UT>(option) }
        {
        }

        OptionSet(std::initializer_list<T> options)
        {
            for (auto& option : options)
                _value |= static_cast<UT>(option);
        }

        void add(T option) { _value |= static_cast<UT>(option); }
        void remove(T option) { _value &= ~static_cast<UT>(option); }

        bool contains(T option) const { return _value & static_cast<UT>(option); }
        bool containsAny(OptionSet<T> options) const { return _value & options._value; }
        bool containsOnly(OptionSet<T> options) const { return _value == options._value; }
        bool containsAll(OptionSet<T> options) const { return (_value & options._value) == options._value; }

        template<typename... Args>
        bool containsAny(Args... optons) const { return containsAny({ optons... }); }
        template<typename... Args>
        bool containsOnly(Args... optons) const { return containsOnly({ optons... }); }
        template<typename... Args>
        bool containsAll(Args... optons) const { return containsAll({ optons... }); }

        operator bool() const { return _value; }

    private:
        OptionSet(UT value) :
            _value { value }
        {
        }

        UT _value { 0 };
    };
}

using Core::OptionSet;
