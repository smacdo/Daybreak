#include "stdafx.h"
#include "app/timespan.h"

#include <sstream>

#include "../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("time constructed with no parameters is set to zero", "[app][support][timespan_t]")
{
    auto t = timespan_t();
    REQUIRE(0 == t.rawValue());
}

TEST_CASE("time can be copy constructed", "[app][support][timespan_t]")
{
    auto t = timespan_t::fromSeconds(1.4);
    timespan_t copy(t);

    REQUIRE(1.4 == copy.totalSeconds());
    REQUIRE(1.4 == t.totalSeconds());
}

TEST_CASE("time can be created in milliseconds", "[app][support][timespan_t]")
{
    SECTION("with a whole value")
    {
        auto t = timespan_t::fromMilliseconds(12750);

        REQUIRE(12.750 == t.totalSeconds());
        REQUIRE(12750 == t.totalMilliseconds());
    }

    SECTION("with a fractional value")
    {
        auto t = timespan_t::fromMilliseconds(1500.0);

        REQUIRE(1.5 == t.totalSeconds());
        REQUIRE(1500.0 == t.totalMilliseconds());
    }

    SECTION("with a negative value")
    {
        auto t = timespan_t::fromMilliseconds(-1200);

        REQUIRE(-1.200 == t.totalSeconds());
        REQUIRE(-1200 == t.totalMilliseconds());
    }
}

TEST_CASE("time can be created in seconds", "[app][support][timespan_t]")
{
    SECTION("with a whole value")
    {
        auto t = timespan_t::fromSeconds(61);

        REQUIRE(61.0 == t.totalSeconds());
        REQUIRE(61000.0 == t.totalMilliseconds());
    }

    SECTION("with a fractional value")
    {
        auto t = timespan_t::fromSeconds(1.2);

        REQUIRE(1.2 == t.totalSeconds());
        REQUIRE(1200.0 == t.totalMilliseconds());
    }

    SECTION("with a negative value")
    {
        auto t = timespan_t::fromSeconds(-4);

        REQUIRE(-4.0 == t.totalSeconds());
        REQUIRE(-4000.0 == t.totalMilliseconds());
    }
}

TEST_CASE("time can be created as minutes", "[app][support][timespan_t]")
{
    auto t = timespan_t::fromMinutes(2);

    REQUIRE(2.0 == t.totalMinutes());
    REQUIRE(120.0 == t.totalSeconds());
    REQUIRE(120000.0 == t.totalMilliseconds());
}

TEST_CASE("time can be created as hours", "[app][support][timespan_t]")
{
    auto t = timespan_t::fromMinutes(3);

    REQUIRE(3.0 == t.totalMinutes());
    REQUIRE(180.0 == t.totalSeconds());
    REQUIRE(180000.0 == t.totalMilliseconds());
    REQUIRE(180000000.0 == t.totalMicroseconds());
}

TEST_CASE("time can be created as days", "[app][support][timespan_t]")
{
    SECTION("as a small number")
    {
        auto t = timespan_t::fromDays(5);

        REQUIRE(7200 == t.totalMinutes());
        REQUIRE(432000 == t.totalSeconds());
        REQUIRE(432000000 == t.totalMilliseconds());
        REQUIRE(4.3200E+11 == t.totalMicroseconds());
    }

    SECTION("as a larger number")
    {
        auto t = timespan_t::fromDays(1000);
        REQUIRE(8.6400E+10 == t.totalMilliseconds());
        REQUIRE(8.6400E+13 == t.totalMicroseconds());
    }

    SECTION("throws an exception if it is too large to be stored", "[app][support][timespan_t]")
    {
        auto t = timespan_t::fromDays(106751991);

        REQUIRE_THROWS_WITH(
            [&t] { t = timespan_t::fromDays(106751991 + 1); }(),
            Catch::Contains("Converting from given number of days would result in overflow"));
    }

    SECTION("throws an exception if it is too small to be stored", "[app][support][timespan_t]")
    {
        auto t = timespan_t::fromDays(-106751991);

        REQUIRE_THROWS_WITH(
            [&t] { t = timespan_t::fromDays(-106751991 - 1); }(),
            Catch::Contains("Converting from given number of days would result in underflow"));
    }
}

TEST_CASE("time can be created as a mixture of units", "[app][support][timespan_t]")
{
    auto t = timespan_t::fromTime(3, 5, 22, 30, 10);

    REQUIRE(3 == t.days());
    REQUIRE(5 == t.hours());
    REQUIRE(22 == t.minutes());
    REQUIRE(30 == t.seconds());
    REQUIRE(10 == t.milliseconds());

    // @TODO: Investigate why this is slightly off 278550.022 == 278550.01
    //REQUIRE(278550.022 == t.totalSeconds());
}

TEST_CASE("time can be constructed from raw value", "[app][support][timespan_t]")
{
    auto t = timespan_t::fromRawValue(7152);
    REQUIRE(7152 == t.rawValue());
}

TEST_CASE("time duration is always the time but positive", "[app][support][timespan_t]")
{
    SECTION("zero minutes")
    {
        REQUIRE(0.0 == timespan_t::fromMinutes(0).duration().totalSeconds());
    }

    SECTION("one second")
    {
        REQUIRE(1.0 == timespan_t::fromSeconds(1).duration().totalSeconds());
    }

    SECTION("negative three seconds")
    {
        REQUIRE(3.0 == timespan_t::fromSeconds(-3).duration().totalSeconds());
    }
}

TEST_CASE("time can be tested for emptiness", "[app][support][timespan_t]")
{
    SECTION("true when time is zero")
    {
        REQUIRE(timespan_t::fromRawValue(0).isEmpty());
    }

    SECTION("false when time is greater than zero")
    {
        REQUIRE(!timespan_t::fromRawValue(1).isEmpty());
    }

    SECTION("false when time is less than zero")
    {
        REQUIRE(!timespan_t::fromRawValue(-1).isEmpty());
    }
}

TEST_CASE("time can be added in specific amounts", "[app][support][timespan_t]")
{
    SECTION("adding microseconds")
    {
        REQUIRE(213 == timespan_t::fromRawValue(200).addMicroseconds(13).totalMicroseconds());
    }

    SECTION("adding milliseconds")
    {
        REQUIRE(1.025 == timespan_t::fromSeconds(1).addMilliseconds(25).totalSeconds());
    }

    SECTION("adding seconds")
    {
        REQUIRE(40.350 == timespan_t::fromMilliseconds(350).addSeconds(40).totalSeconds());
    }

    SECTION("adding minutes")
    {
        REQUIRE(4920 == timespan_t::fromHours(1).addMinutes(22).totalSeconds());
    }

    SECTION("adding hours")
    {
        REQUIRE(115200 == timespan_t::fromHours(2).addHours(30).totalSeconds());
    }

    SECTION("adding days")
    {
        REQUIRE(93600 == timespan_t::fromHours(2).addDays(1).totalSeconds());
    }
}

TEST_CASE("time can be reported in seconds", "[app][support][timespan_t]")
{
    REQUIRE(1.2 == timespan_t::fromSeconds(1.2).duration().totalSeconds());
    REQUIRE(1.2 == timespan_t::fromMilliseconds(1200).duration().totalSeconds());
}

TEST_CASE("time can be reported in milliseconds", "[app][support][timespan_t]")
{
    REQUIRE(1200.0 == timespan_t::fromSeconds(1.2).duration().totalMilliseconds());
    REQUIRE(1200.0 == timespan_t::fromMilliseconds(1200).duration().totalMilliseconds());
}

TEST_CASE("time can be swapped", "[app][support][timespan_t]")
{
    auto a = timespan_t::fromSeconds(5.0);
    auto b = timespan_t::fromSeconds(6.0);

    REQUIRE(5.0 == a.totalSeconds());
    REQUIRE(6.0 == b.totalSeconds());

    swap(a, b);

    REQUIRE(6.0 == a.totalSeconds());
    REQUIRE(5.0 == b.totalSeconds());
}

TEST_CASE("time can be checked for equality", "[app][support][timespan_t]")
{
    auto a = timespan_t::fromSeconds(10.0);
    auto b = timespan_t::fromSeconds(20.0) / 2.0;
    auto c = timespan_t::fromSeconds(10.1);

    REQUIRE(a == a);
    REQUIRE(a == b);
    REQUIRE(b == a);
    REQUIRE(!(a == c));
}

TEST_CASE("time can be checked for inequality", "[app][support][timespan_t]")
{
    auto a = timespan_t::fromSeconds(10.0);
    auto b = timespan_t::fromSeconds(20.0) / 2.0;
    auto c = timespan_t::fromSeconds(10.1);

    REQUIRE(a != c);
    REQUIRE(c != a);

    REQUIRE(!(a != a));
    REQUIRE(!(a != b));
    REQUIRE(!(b != a));
}

TEST_CASE("time can be assigned", "[app][support][timespan_t]")
{
    auto t = timespan_t::fromSeconds(1.5);
    auto copy = timespan_t::fromSeconds(0);

    REQUIRE(0.0 == copy.totalSeconds());

    copy = t;

    REQUIRE(1.5 == copy.totalSeconds());
}

TEST_CASE("time can be cmopared", "[app][support][timespan_t]")
{
    auto a = timespan_t::fromSeconds(-10.0);
    auto b = timespan_t::fromSeconds(25.0);
    auto c = timespan_t::fromSeconds(30.0);

    SECTION("with less than")
    {
        REQUIRE(a < b);
        REQUIRE(!(b < a));
        REQUIRE(a < c);
        REQUIRE(!(c < b));
    }
    
    SECTION("with less than equals")
    {
        REQUIRE(a <= b);
        REQUIRE(a <= a);
        REQUIRE(!(b <= a));
    }

    SECTION("with greater than")
    {
        REQUIRE(b > a);
        REQUIRE(!(a > b));
        REQUIRE(c > b);
        REQUIRE(!(b > c));
        REQUIRE(c > a);
        REQUIRE(!(a > c));
    }

    SECTION("with greater than equals")
    {
        REQUIRE(b >= a);
        REQUIRE(b >= b);
        REQUIRE(c >= c);
        REQUIRE(!(a >= b));
    }
}

TEST_CASE("time can be negated", "[app][support][timespan_t]")
{
    REQUIRE(-1.2 == -timespan_t::fromSeconds(1.2).totalSeconds());
    REQUIRE(1.2 == -timespan_t::fromSeconds(-1.2).totalSeconds());
}

TEST_CASE("time with unary plus does nothing", "[app][support][timespan_t]")
{
    REQUIRE(1.2 == +timespan_t::fromSeconds(1.2).totalSeconds());
    REQUIRE(-1.2 == +timespan_t::fromSeconds(-1.2).totalSeconds());
}

TEST_CASE("time can be added", "[app][support][timespan_t]")
{
    SECTION("with the + operator")
    {
        auto t = timespan_t::fromSeconds(2.0) + timespan_t::fromSeconds(0.8);
        REQUIRE(2.8 == t.totalSeconds());
    }

    SECTION("with the += operator")
    {
        auto t = timespan_t::fromSeconds(3.0);
        t += timespan_t::fromSeconds(0.2);
        REQUIRE(3.2 == t.totalSeconds());
    }
}

TEST_CASE("time can be subtracted", "[app][support][timespan_t]")
{
    SECTION("with the - operator")
    {
        auto t = timespan_t::fromSeconds(2.0) - timespan_t::fromSeconds(0.8);
        REQUIRE(1.2 == t.totalSeconds());
    }

    SECTION("with the -= operator")
    {
        auto t = timespan_t::fromSeconds(-4.0);
        t -= timespan_t::fromSeconds(0.1);
        REQUIRE(-4.1 == t.totalSeconds());
    }
}

TEST_CASE("time can be multiplied", "[app][support][timespan_t]")
{
    SECTION("with the * operator")
    {
        auto t = timespan_t::fromSeconds(2.0) * -2.0;
        REQUIRE(-4.0 == t.totalSeconds());
    }

    SECTION("with the *= operator")
    {
        auto t = timespan_t::fromSeconds(2.2);
        t *= 2.0;
        REQUIRE(4.4 == t.totalSeconds());
    }
}

TEST_CASE("time can be divided", "[app][support][timespan_t]")
{
    SECTION("with the / operator")
    {
        auto t = timespan_t::fromSeconds(42.0) / 2.0;
        REQUIRE(21.0 == t.totalSeconds());
    }
    
    SECTION("with the /= operator")
    {
        auto t = timespan_t::fromSeconds(30.0);
        t /= -3.0;
        REQUIRE(-10.0 == t.totalSeconds());
    }
}

TEST_CASE("time can be created as a user literal")
{
    SECTION("in seconds")
    {
        auto t = 1.3_s;
        REQUIRE(1.3 == t.totalSeconds());
    }

    SECTION("in miliseconds")
    {
        auto t = 1400.0_ms;
        REQUIRE(1.4 == t.totalSeconds());
    }
}

TEST_CASE("time can be converted to a string")
{
    auto t = timespan_t::fromMinutes(62).addSeconds(10);
    REQUIRE("1.036111 hour(s)" == std::to_string(t));
}

TEST_CASE("time can be written to a ostream")
{
    // 3720 + 10 = 3730 seconds
    // 3730 min => 2.2380E+11
    auto t = timespan_t::fromMinutes(62).addSeconds(10);

    std::stringstream ss;
    ss << t;
    
    auto s = ss.str();

    REQUIRE("3730000000" == s);
}

// @TODO: Test overflow / underflow when using +, -, *, / operators.