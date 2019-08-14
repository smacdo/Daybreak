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
#include "Time.h"

#include <ostream>
#include <sstream>
#include <iomanip>

using namespace Daybreak;

const int64_t TimeSpan::MicrosecondsPerMillisecond = 1000LL;
const int64_t TimeSpan::MicrosecondsPerSecond = 1000LL * 1000;				// 1e6 or 1000 milliseconds
const int64_t TimeSpan::MicrosecondsPerMinute = 1000LL * 1000 * 60;			// 6e7 or 60 seconds.
const int64_t TimeSpan::MicrosecondsPerHour = 1000LL * 1000 * 60 * 60;		// 3.6e9 or 60 minutes.
const int64_t TimeSpan::MicrosecondsPerDay = 1000LL * 1000 * 60 * 60 * 24;	// 8.64e10 or 24 hours.

const int32_t TimeSpan::MaxRepresentableDays = 106751991;
const int32_t TimeSpan::MinRepresentableDays = -106751991;

//---------------------------------------------------------------------------------------------------------------------
namespace Daybreak
{
    //-----------------------------------------------------------------------------------------------------------------
    void swap(TimeSpan& a, TimeSpan& b) noexcept
    {
        using std::swap;
        swap(a.m_microseconds, b.m_microseconds);
    }

    //-----------------------------------------------------------------------------------------------------------------
    std::string to_string(const TimeSpan& t)
    {
        std::stringstream ss;

        ss.setf(std::ios::fixed);
        ss << std::setprecision(6);

        if (t.m_microseconds < TimeSpan::MicrosecondsPerMillisecond)
        {
            ss << t.totalMicroseconds() << " usec";
        }
        else if (t.m_microseconds < TimeSpan::MicrosecondsPerSecond)
        {
            ss << t.totalMilliseconds() << " msec";
        }
        else if (t.m_microseconds < TimeSpan::MicrosecondsPerMinute)
        {
            ss << t.totalSeconds() << " sec";
        }
        else if (t.m_microseconds < TimeSpan::MicrosecondsPerHour)
        {
            ss << t.totalMinutes() << " min";
        }
        else if (t.m_microseconds < TimeSpan::MicrosecondsPerDay)
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
    std::ostream& operator <<(std::ostream& os, const TimeSpan& t)
    {
        os << to_string(t);
        return os;
    }

}

//---------------------------------------------------------------------------------------------------------------------
namespace std
{
    /** Returns a string with the representation of val. */
    std::string to_string(const Daybreak::TimeSpan& val)
    {
        return Daybreak::to_string(val);
    }
}
