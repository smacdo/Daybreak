/*
 * Copyright 2012 - 2019 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
     * is converted to a floating point value with a potential loss of precision. Quick tests show that at 1000 days
     * there is still no loss of precision with totalMiliseconds() or totalMicroseconds() level but more testing is
     * needed to find when precision starts being lost.
     * (eg how much time before microseconds as a double is no longer precise, totalMilliseconds(), etc).
     *
     * @TODO Calculate when the precision loss occurs and write unit tests to confirm.
     */
    class timespan_t
    {
    public:
        /** Initialize time span to zero seconds. */
        constexpr timespan_t() noexcept
            : m_microseconds(0LL)
        {
        }

        /** Copy constructor. */
        constexpr timespan_t(const timespan_t&) noexcept = default;

    protected:
        /**
         * Initialize timespan to given number of microseconds. Use static constructor methods such as FromSeconds,
         * FromMinutes to construct time spans with values other than microseconds.
         */
        constexpr explicit timespan_t(int64_t microseconds) noexcept
            : m_microseconds(microseconds)
        {
        }

    public:
        /** Create timespan from milliseconds. */
        static constexpr timespan_t fromMilliseconds(int32_t milliseconds) noexcept
        {
            return timespan_t(MicrosecondsPerMillisecond * milliseconds);
        }

        /** Create timespan from milliseconds. */
        static constexpr timespan_t fromMilliseconds(double milliseconds) noexcept
        {
            // Convert the given seconds into microseconds and truncate any partial microseconds.
            return timespan_t(static_cast<int32_t>(MicrosecondsPerMillisecond * milliseconds));
        }

        /** Create timespan from seconds. */
        static constexpr timespan_t fromSeconds(int32_t seconds) noexcept
        {
            return timespan_t(MicrosecondsPerSecond * seconds);
        }

        /** Create timespan from seconds. */
        static constexpr timespan_t fromSeconds(double seconds) noexcept
        {
            // Convert the given seconds into microseconds and truncate any partial microseconds.
            return timespan_t(static_cast<int32_t>(MicrosecondsPerSecond * seconds));
        }

        /** Create timespan from minutes. */
        static constexpr timespan_t fromMinutes(int32_t minutes) noexcept
        {
            return timespan_t(MicrosecondsPerMinute * minutes);
        }

        /** Create timespan from hours. */
        static constexpr timespan_t fromHours(int32_t hours) noexcept
        {
            // Max hours (2.56204779E9) is larger than int32 so no need to check for overflow or underflow.
            return timespan_t(MicrosecondsPerHour * hours);
        }

        /** Create timespan from days. */
        static timespan_t fromDays(int32_t days)
        {
            if (days > MaxRepresentableDays)
            {
                throw std::runtime_error("Converting from given number of days would result in overflow");
            }
            else if (days < MinRepresentableDays)
            {
                throw std::runtime_error("Converting from given number of days would result in underflow");
            }

            return timespan_t(MicrosecondsPerDay * days);
        }

        /** Create timespan from given units of time. */
        static timespan_t fromTime(
            int32_t days,               ///< The number of days to add.
            int32_t hours,              ///< The number of hours to add.
            int32_t minutes,            ///< The number of minutes to add.
            int32_t seconds,            ///< The number of seconds to add.
            int32_t milliseconds = 0)   ///< The number of milliseconds to add.
        {
            return timespan_t()
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
        static constexpr timespan_t fromRawValue(int64_t rawValue) noexcept
        {
            return timespan_t(rawValue);
        }

    public:
        /** Get the duration of a timespan, which is always the absolute value of the time span. */
        constexpr timespan_t duration() const noexcept
        {
            return timespan_t(std::abs(m_microseconds));
        }

        /** Get raw time data from timespan, suitable for serialization. */
        constexpr int64_t rawValue() const noexcept
        {
            return m_microseconds;
        }

        /** Get the microseconds component of the time span. */
        constexpr int64_t microseconds() const noexcept
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
            // TODO: How to increase accuracy? Is this operation losing accuracy?
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
            // TODO: How to increase accuracy? Is this operation losing accuracy?
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
            // TODO: How to increase accuracy? Is this operation losing accuracy?
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
            // TODO: How to increase accuracy? Is this operation losing accuracy?
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
        timespan_t addMicroseconds(int64_t microseconds) const
        {
            return *this + timespan_t(microseconds);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of milliseconds. */
        timespan_t addMilliseconds(int32_t milliseconds) const
        {
            return *this + timespan_t::fromMilliseconds(milliseconds);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of seconds. */
        timespan_t addSeconds(int32_t seconds) const
        {
            return *this + timespan_t::fromSeconds(seconds);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of minutes. */
        timespan_t addMinutes(int32_t minutes) const
        {
            return *this + timespan_t::fromMinutes(minutes);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of hours. */
        timespan_t addHours(int32_t hours) const
        {
            return *this + timespan_t::fromHours(hours);
        }

        /** Get a new timespan that is the result of adding this timepsan and the given number of days. */
        timespan_t addDays(int32_t days) const
        {
            return *this + timespan_t::fromDays(days);
        }

    public:
        friend std::string to_string(const timespan_t& t);
        friend void swap(timespan_t& a, timespan_t& b) noexcept;

        /** Copy assignment operator. */
        constexpr timespan_t& operator =(const timespan_t&) noexcept = default;

        /** Equality operator overload. */
        constexpr friend bool operator ==(const timespan_t& lhs, const timespan_t& rhs) noexcept
        {
            return lhs.m_microseconds == rhs.m_microseconds;
        }

        /** Inequality operator overload. */
        constexpr friend bool operator !=(const timespan_t& lhs, const timespan_t& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        /** Less than operator overload. */
        constexpr friend bool operator <(const timespan_t& lhs, const timespan_t& rhs) noexcept
        {
            return lhs.m_microseconds < rhs.m_microseconds;
        }

        /** Less than or equal to operator overload. */
        constexpr friend bool operator <=(const timespan_t& lhs, const timespan_t& rhs) noexcept
        {
            return lhs.m_microseconds <= rhs.m_microseconds;
        }

        /** Greater than operator overload. */
        constexpr friend bool operator >(const timespan_t& lhs, const timespan_t& rhs) noexcept
        {
            return lhs.m_microseconds > rhs.m_microseconds;
        }

        /** Greater than or equal to operator overload. */
        constexpr friend bool operator >=(const timespan_t& lhs, const timespan_t& rhs) noexcept
        {
            return lhs.m_microseconds >= rhs.m_microseconds;
        }

        /** Unary negation operator overload. */
        friend timespan_t operator -(const timespan_t& lhs)
        {
            return timespan_t(-lhs.m_microseconds);
        }

        /** Unary positive operator overload. */
        friend timespan_t operator +(const timespan_t& lhs)
        {
            return timespan_t(+lhs.m_microseconds);
        }

        /** Self addition operator overload. Overflow and underflow will throw an exception. */
        timespan_t& operator +=(const timespan_t& rhs)
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
        timespan_t& operator -=(const timespan_t& rhs)
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
        timespan_t& operator *=(double factor)
        {
            // TODO: Detect overflow and underflow.
            m_microseconds = static_cast<int64_t>(m_microseconds * factor);
            return *this;
        }

        /** Self division operator overload. */
        timespan_t& operator /=(double factor)
        {
            // TODO: Detect overflow and underflow.
            m_microseconds = static_cast<int64_t>(m_microseconds / factor);
            return *this;
        }

        /** Addition operator overload. */
        friend timespan_t operator +(timespan_t lhs, const timespan_t& rhs)
        {
            lhs += rhs;
            return lhs;
        }

        /** Subtraction operator overload. */
        friend timespan_t operator -(timespan_t lhs, const timespan_t& rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        /** Multiplication operator overload. */
        friend timespan_t operator *(timespan_t lhs, double factor)
        {
            lhs *= factor;
            return lhs;
        }

        /** Division operator overload. */
        friend timespan_t operator /(timespan_t lhs, double factor)
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
    timespan_t constexpr operator"" _s(long double seconds) noexcept
    {
        return timespan_t::fromSeconds(static_cast<double>(seconds));
    }

    /** User literal for time in milliseconds. */
    timespan_t constexpr operator"" _ms(long double milliseconds) noexcept
    {
        return timespan_t::fromMilliseconds(static_cast<double>(milliseconds));
    }

    /** Swap two timespan values. */
    void swap(timespan_t& a, timespan_t& b) noexcept;

    /** Get the value of the timespan expressed as a human readable string. */
    std::string to_string(const timespan_t& t);

    /** TimeSpan ostream overload - prints the time to an output string. */
    std::ostream& operator << (std::ostream& os, const timespan_t& t);
}

namespace std
{
    /** Returns a string with the representation of val. */
    std::string to_string(const Daybreak::timespan_t& val);
}
