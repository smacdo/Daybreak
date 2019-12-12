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
#include "stdafx.h"
#include "timespan.h"

#include <ostream>
#include <sstream>
#include <iomanip>

using namespace Daybreak;

const int64_t timespan_t::MicrosecondsPerMillisecond = 1000LL;
const int64_t timespan_t::MicrosecondsPerSecond = 1000LL * 1000;				// 1e6 or 1000 milliseconds
const int64_t timespan_t::MicrosecondsPerMinute = 1000LL * 1000 * 60;			// 6e7 or 60 seconds.
const int64_t timespan_t::MicrosecondsPerHour = 1000LL * 1000 * 60 * 60;		// 3.6e9 or 60 minutes.
const int64_t timespan_t::MicrosecondsPerDay = 1000LL * 1000 * 60 * 60 * 24;	// 8.64e10 or 24 hours.

const int32_t timespan_t::MaxRepresentableDays = 106751991;
const int32_t timespan_t::MinRepresentableDays = -106751991;

//---------------------------------------------------------------------------------------------------------------------
namespace Daybreak
{
    //-----------------------------------------------------------------------------------------------------------------
    void swap(timespan_t& a, timespan_t& b) noexcept
    {
        using std::swap;
        swap(a.m_microseconds, b.m_microseconds);
    }

    //-----------------------------------------------------------------------------------------------------------------
    std::string to_string(const timespan_t& t)
    {
        std::stringstream ss;

        ss.setf(std::ios::fixed);
        ss << std::setprecision(6);

        if (t.m_microseconds < timespan_t::MicrosecondsPerMillisecond)
        {
            ss << t.totalMicroseconds() << " usec";
        }
        else if (t.m_microseconds < timespan_t::MicrosecondsPerSecond)
        {
            ss << t.totalMilliseconds() << " msec";
        }
        else if (t.m_microseconds < timespan_t::MicrosecondsPerMinute)
        {
            ss << t.totalSeconds() << " sec";
        }
        else if (t.m_microseconds < timespan_t::MicrosecondsPerHour)
        {
            ss << t.totalMinutes() << " min";
        }
        else if (t.m_microseconds < timespan_t::MicrosecondsPerDay)
        {
            ss << t.totalHours() << " hour(s)";
        }
        else
        {
            ss << t.totalDays() << " day(s)";
        }

        return ss.str();
    }

    //-----------------------------------------------------------------------------------------------------------------
    std::ostream& operator <<(std::ostream& os, const timespan_t& t)
    {
        using std::to_string;
        os << to_string(t.rawValue());
        return os;
    }
}

//---------------------------------------------------------------------------------------------------------------------
namespace std
{
    /** Returns a string with the representation of val. */
    std::string to_string(const Daybreak::timespan_t& val)
    {
        return Daybreak::to_string(val);
    }
}
