#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

namespace Daybreak
{
    /** 
     * Represents a high precision time interval that is measured as a positive or negative span of time. Internally
     * the time is tracked as microseconds (one ten millionth of a second) with a signed 64 integer. This allows a
     * maximum of 106 million days  (106,751,991 days to be precise) which should be enough for most use cases.
     *
     * TimeSpan will throw runtime_error exceptions when integer overflow or underflow is detected.
     *
     * When converting to units of time other than microseconds, for example totalSeconds() or totalMinutes() the time
     * is converted to a floating point value with a potential loss of precision.
     */
    class TimeSpan
    {
    public:
        /** Initialize time span to zero seconds. */
        constexpr TimeSpan() noexcept
            : m_microseconds(0LL)
        {
        }

    protected:
        /** 
         * Initialize timespan to given number of microseconds. Use static constructor methods such as FromSeconds,
         * FromMinutes to construct time spans with values other than microseconds.
         */
        constexpr explicit TimeSpan(int64_t microseconds) noexcept
            : m_microseconds(microseconds)
        {
        }

    public:
        /** Create timespan from milliseconds. */
        static constexpr TimeSpan fromMilliseconds(int32_t milliseconds) noexcept
        {
            return TimeSpan(MicrosecondsPerMillisecond * milliseconds);
        }

        /** Create timespan from milliseconds. */
        static constexpr TimeSpan fromMilliseconds(double milliseconds) noexcept
        {
            // Convert the given seconds into microseconds and truncate any partial microseconds.
            return TimeSpan(static_cast<int32_t>(MicrosecondsPerMillisecond * milliseconds));
        }

        /** Create timespan from seconds. */
        static constexpr TimeSpan fromSeconds(int32_t seconds) noexcept
        {
            return TimeSpan(MicrosecondsPerSecond * seconds);
        }

        /** Create timespan from seconds. */
        static constexpr TimeSpan fromSeconds(double seconds) noexcept
        {
            // Convert the given seconds into microseconds and truncate any partial microseconds.
            return TimeSpan(static_cast<int32_t>(MicrosecondsPerSecond * seconds));
        }

        /** Create timespan from minutes. */
        static constexpr TimeSpan fromMinutes(int32_t minutes) noexcept
        {
            return TimeSpan(MicrosecondsPerMinute * minutes);
        }

        /** Create timespan from hours. */
        static constexpr TimeSpan fromHours(int32_t hours) noexcept
        {
            // Max hours (2.56204779E9) is larger than int32 so no need to check for overflow or underflow.
            return TimeSpan(MicrosecondsPerHour * hours);
        }

        /** Create timespan from days. */
        static constexpr TimeSpan fromDays(int32_t days)
        {
            if (days > MaxRepresentableDays)
            {
                throw std::runtime_error("Converting from given number of days would result in overflow");
            }
            else if (days < MinRepresentableDays)
            {
                throw std::runtime_error("Converting from given number of days would result in underflow");
            }

            return TimeSpan(MicrosecondsPerDay * days);
        }

        /** Create timespan from given time. */
        static constexpr TimeSpan fromTime(
            int32_t days,
            int32_t hours,
            int32_t minutes,
            int32_t seconds,
            int32_t milliseconds = 0)
        {
            return TimeSpan()
                .addDays(days)
                .addHours(hours)
                .addMinutes(minutes)
                .addSeconds(seconds)
                .addMilliseconds(milliseconds);
        }

        /** 
         * Create timespan from raw value.
         * Only use values obtained from rawValue() otherwise results are undefined. 
         */
        static constexpr TimeSpan fromRawValue(int64_t rawValue) noexcept
        {
            return TimeSpan(rawValue);
        }

    public:
        /** Get the duration of a timespan, which is always the absolute value of the time span. */
        constexpr TimeSpan duration() const noexcept
        {
            return TimeSpan(std::abs(m_microseconds));
        }

        /** Get raw time data from timespan, suitable for serialization. */
        constexpr int64_t rawValue() const noexcept
        {
            return m_microseconds;
        }

        /** Get the microseconds component of the time span. */
        constexpr int32_t microseconds() const noexcept
        {
            return static_cast<int64_t>(m_microseconds % 1000);
        }

        /** Get the value of the timespan expressed in microseconds. */
        constexpr int64_t totalMicroseconds() const noexcept
        {
            return m_microseconds;
        }

        /** Get the milliseconds component of the time span. */
        constexpr int32_t milliseconds() const noexcept
        {
            return static_cast<int32_t>((m_microseconds / MicrosecondsPerMillisecond) % 1000);
        }

        /** Get the value of the timespan expressed in whole and fractional millieconds. */
        constexpr double totalMilliseconds() const noexcept
        {
            return static_cast<double>(m_microseconds) / static_cast<double>(MicrosecondsPerMillisecond);
        }

        /** Get the seconds component of the time span. */
        constexpr int32_t seconds() const noexcept
        {
            return static_cast<int32_t>((m_microseconds / MicrosecondsPerSecond) % 60);
        }

        /** Get the value of the timespan expressed in whole and fractional seconds. */
        constexpr double totalSeconds() const noexcept
        {
            return static_cast<double>(m_microseconds) / static_cast<double>(MicrosecondsPerSecond);
        }

        /** Get the minutes component of the time span. */
        constexpr int32_t minutes() const noexcept
        {
            return static_cast<int32_t>((m_microseconds / MicrosecondsPerMinute) % 60);
        }

        /** Get the value of the timespan expressed in whole and fractional minutes. */
        constexpr double totalMinutes() const noexcept
        {
            return static_cast<double>(m_microseconds) / static_cast<double>(MicrosecondsPerMinute);
        }

        /** Get the hours component of the time span. */
        constexpr int32_t hours() const noexcept
        {
            return static_cast<int32_t>((m_microseconds / MicrosecondsPerHour) % 24);
        }

        /** Get the value of the timespan expressed in whole and fractional hours. */
        constexpr double totalHours() const noexcept
        {
            return static_cast<double>(m_microseconds) / static_cast<double>(MicrosecondsPerHour);
        }

        /** Get the days component of the time span. */
        constexpr int32_t days() const noexcept
        {
            return static_cast<int32_t>(m_microseconds / MicrosecondsPerDay);
        }

        /** Get the value of the timespan expressed in whole and fractional days. */
        constexpr double totalDays() const noexcept
        {
            return static_cast<double>(m_microseconds) / static_cast<double>(MicrosecondsPerDay);
        }

        /** Check if the time is empty (zero). */
        constexpr bool isEmpty() const noexcept
        {
            return m_microseconds == 0;
        }

    public:
        /** Get a new timespan that is the result of adding this timepsan and the given number of microseconds. */
        constexpr TimeSpan addMicroseconds(int64_t microseconds) const
        {
            return *this + TimeSpan(microseconds);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of milliseconds. */
        constexpr TimeSpan addMilliseconds(int32_t milliseconds) const
        {
            return *this + TimeSpan::fromMilliseconds(milliseconds);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of seconds. */
        constexpr TimeSpan addSeconds(int32_t seconds) const
        {
            return *this + TimeSpan::fromSeconds(seconds);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of minutes. */
        constexpr TimeSpan addMinutes(int32_t minutes) const
        {
            return *this + TimeSpan::fromMinutes(minutes);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of hours. */
        constexpr TimeSpan addHours(int32_t hours) const
        {
            return *this + TimeSpan::fromHours(hours);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of days. */
        constexpr TimeSpan addDays(int32_t days) const
        {
            return *this + TimeSpan::fromDays(days);
        }

    public:
        friend std::string to_string(const TimeSpan& t);
        friend void swap(TimeSpan& a, TimeSpan& b) noexcept;

        /** Equality operator overload. */
        constexpr friend bool operator ==(const TimeSpan& lhs, const TimeSpan& rhs) noexcept
        {
            return lhs.m_microseconds == rhs.m_microseconds;
        }

        /** Inequality operator overload. */
        constexpr friend bool operator !=(const TimeSpan& lhs, const TimeSpan& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        /** Less than operator overload. */
        constexpr friend bool operator <(const TimeSpan& lhs, const TimeSpan& rhs) noexcept
        {
            return lhs.m_microseconds < rhs.m_microseconds;
        }

        /** Less than or equal to operator overload. */
        constexpr friend bool operator <=(const TimeSpan& lhs, const TimeSpan& rhs) noexcept
        {
            return lhs.m_microseconds <= rhs.m_microseconds;
        }

        /** Greater than operator overload. */
        constexpr friend bool operator >(const TimeSpan& lhs, const TimeSpan& rhs) noexcept
        {
            return lhs.m_microseconds > rhs.m_microseconds;
        }

        /** Greater than or equal to operator overload. */
        constexpr friend bool operator >=(const TimeSpan& lhs, const TimeSpan& rhs) noexcept
        {
            return lhs.m_microseconds >= rhs.m_microseconds;
        }

        /** Unary negation operator overload. */
        constexpr friend TimeSpan operator -(const TimeSpan& lhs)
        {
            return TimeSpan(-lhs.m_microseconds);
        }

        /** Unary positive operator overload. */
        constexpr friend TimeSpan operator +(const TimeSpan& lhs)
        {
            return TimeSpan(+lhs.m_microseconds);
        }

        /** Self addition operator overload. Overflow and underflow will throw an exception. */
        constexpr TimeSpan& operator +=(const TimeSpan& rhs)
        {
            auto l = m_microseconds;
            auto r = rhs.m_microseconds;

            if (r > 0 && l > std::numeric_limits<int64_t>::max() - r)
            {
                throw std::runtime_error("TimeSpan addition resulted in overflow");
            }
            else if (r < 0 && l < std::numeric_limits<int64_t>::min() - r)
            {
                throw std::runtime_error("TimeSpan addition resulted in underflow");
            }

            m_microseconds += r;
            return *this;
        }

        /** Self subtraction operator overload. */
        constexpr TimeSpan& operator -=(const TimeSpan& rhs)
        {
            auto l = m_microseconds;
            auto r = rhs.m_microseconds;

            if (r < 0 && l > std::numeric_limits<int64_t>::max() + r)
            {
                throw std::runtime_error("TimeSpan subtraction resulted in overflow");
            }
            else if (r > 0 && l < std::numeric_limits<int64_t>::min() + r)
            {
                throw std::runtime_error("TimeSpan subtraction resulted in underflow");
            }

            m_microseconds -= r;
            return *this;
        }

        /** Self multiplication operator overload. */
        constexpr TimeSpan& operator *=(double factor)
        {
            // TODO: Detect overflow and underflow.
            m_microseconds = static_cast<int64_t>(m_microseconds * factor);
            return *this;
        }

        /** Self division operator overload. */
        constexpr TimeSpan& operator /=(double factor)
        {
            // TODO: Detect overflow and underflow.
            m_microseconds = static_cast<int64_t>(m_microseconds / factor);
            return *this;
        }

        /** Addition operator overload. */
        constexpr friend TimeSpan operator +(TimeSpan lhs, const TimeSpan& rhs)
        {
            lhs += rhs;
            return lhs;
        }
        
        /** Subtraction operator overload. */
        constexpr friend TimeSpan operator -(TimeSpan lhs, const TimeSpan& rhs)
        {
            lhs -= rhs;
            return lhs;
        }
        
        /** Multiplication operator overload. */
        constexpr friend TimeSpan operator *(TimeSpan lhs, double factor)
        {
            lhs *= factor;
            return lhs;
        }
        
        /** Division operator overload. */
        constexpr friend TimeSpan operator /(TimeSpan lhs, double factor)
        {
            lhs /= factor;
            return lhs;
        }

    public:
        const static int64_t MicrosecondsPerMillisecond;
        const static int64_t MicrosecondsPerSecond;
        const static int64_t MicrosecondsPerMinute;
        const static int64_t MicrosecondsPerHour;
        const static int64_t MicrosecondsPerDay;

        const static int32_t MaxRepresentableDays;
        const static int32_t MinRepresentableDays;

    private:
        int64_t m_microseconds = 0;
    };

    /** User literal for time in seconds. */
    TimeSpan constexpr operator"" _s(long double seconds) noexcept
    {
        return TimeSpan::fromSeconds(static_cast<double>(seconds));
    }

    /** User literal for time in milliseconds. */
    TimeSpan constexpr operator"" _ms(long double milliseconds) noexcept
    {
        return TimeSpan::fromMilliseconds(static_cast<double>(milliseconds));
    }
    
    /** Swap two timespan values. */
    void swap(TimeSpan& a, TimeSpan& b) noexcept;

    /** Get the value of the timespan expressed as a human readable string. */
    std::string to_string(const TimeSpan& t);

    /** TimeSpan ostream overload - prints the time to an output string. */
    std::ostream& operator << (std::ostream& os, const TimeSpan& t);
}

namespace std
{
    /** Returns a string with the representation of val. */
    std::string to_string(const Daybreak::TimeSpan& val);
}
