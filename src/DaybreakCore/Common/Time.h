#pragma once
#include <utility>
#include <cmath>

namespace Daybreak
{
    // Represents an interval of time, internally tracked as seconds.
    class TimeSpan
    {
    public:
        typedef double value_type;

    public:
        // Default constructor.
        TimeSpan() noexcept = default;

    protected:
        // Constructor. Creates new instance of time from specified number of seconds.
        constexpr explicit TimeSpan(value_type seconds) noexcept
            : m_seconds(seconds)
        {
        }

    public:
        // Copy constructor.
        TimeSpan(const TimeSpan&) noexcept = default;

        // Destructor.
        ~TimeSpan() = default;

        // Assignment operator.
        TimeSpan& operator =(TimeSpan other) noexcept
        {
            swap(*this, other);
            return *this;
        }

    public:
        // Get a time whose value is the absolute value of the current time.
        constexpr TimeSpan duration() const noexcept
        {
            return TimeSpan(std::abs(m_seconds));
        }

        // Get time in milliseconds.
        constexpr value_type totalMilliseconds() const noexcept
        {
            return m_seconds * static_cast<value_type>(1000);
        }

        // Get time in seconds.
        constexpr value_type totalSeconds() const noexcept
        {
            return m_seconds;
        }

        // Swap time with another time.
        friend void swap(TimeSpan& a, TimeSpan& b) noexcept
        {
            using std::swap;

            swap(a.m_seconds, b.m_seconds);
        }

    public:
        // Unary negation.
        constexpr TimeSpan operator -() const noexcept
        {
            return TimeSpan(-m_seconds);
        }

        // Unary positation (haha).
        constexpr TimeSpan operator +() const noexcept
        {
            return TimeSpan(+m_seconds);
        }

        // Self addition operator.
        constexpr TimeSpan& operator +=(const TimeSpan& rhs) noexcept
        {
            m_seconds += rhs.m_seconds;
            return *this;
        }

        // Self subtraction operator.
        constexpr TimeSpan& operator -=(const TimeSpan& rhs) noexcept
        {
            m_seconds -= rhs.m_seconds;
            return *this;
        }

        // Self multiplication operator.
        constexpr TimeSpan& operator *=(const value_type factor) noexcept
        {
            m_seconds *= factor;
            return *this;
        }

        // Self division operator.
        constexpr TimeSpan& operator /=(const value_type factor) noexcept
        {
            m_seconds /= factor;
            return *this;
        }

    public:
        // Static constructor that creates new time instance from specified number of seconds.
        constexpr static TimeSpan FromSeconds(value_type seconds) noexcept
        {
            return TimeSpan(seconds);
        }

        // Static construct that creates new time instance from specified number of milliseconds.
        constexpr static TimeSpan FromMilliseconds(value_type milliseconds) noexcept
        {
            return TimeSpan(milliseconds / static_cast<value_type>(1000));
        }

    private:
        // Number of seconds that have elapsed.
        value_type m_seconds = 0;
    };

    // User literal for time in seconds.
    TimeSpan constexpr operator"" _s(long double seconds) noexcept
    {
        return  TimeSpan::FromSeconds(static_cast<TimeSpan::value_type>(seconds));
    }

    // User literal for time in seconds.
    TimeSpan constexpr operator"" _ms(long double milliseconds) noexcept
    {
        return  TimeSpan::FromMilliseconds(static_cast<TimeSpan::value_type>(milliseconds));
    }


    // Arithmetic operators.
    inline constexpr TimeSpan operator +(TimeSpan lhs, const TimeSpan& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    inline constexpr TimeSpan operator -(TimeSpan lhs, const TimeSpan& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    inline constexpr TimeSpan operator *(TimeSpan lhs, const TimeSpan::value_type rhs) noexcept
    {
        lhs *= rhs;
        return lhs;
    }

    inline constexpr TimeSpan operator /(TimeSpan lhs, const TimeSpan::value_type rhs) noexcept
    {
        lhs /= rhs;
        return lhs;
    }

    // Comparison operators for time.
    inline constexpr bool operator ==(const TimeSpan& lhs, const TimeSpan& rhs) noexcept
    {
        return lhs.totalSeconds() == rhs.totalSeconds();
    }

    inline constexpr bool operator != (const TimeSpan& lhs, const TimeSpan& rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }

    inline constexpr bool operator < (const TimeSpan& lhs, const TimeSpan& rhs) noexcept
    {
        return lhs.totalSeconds() < rhs.totalSeconds();
    }

    inline constexpr bool operator > (const TimeSpan& lhs, const TimeSpan& rhs) noexcept
    {
        return operator<(rhs, lhs);
    }

    inline constexpr bool operator <= (const TimeSpan& lhs, const TimeSpan& rhs) noexcept
    {
        return !operator>(lhs, rhs);
    }

    inline constexpr bool operator >= (const TimeSpan& lhs, const TimeSpan& rhs) noexcept
    {
        return !operator<(lhs, rhs);
    }
}
