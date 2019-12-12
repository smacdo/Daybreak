#include "stdafx.h"
#include "app/time.h"

#include "../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("Time_Can_Be_Created_In_Seconds", "[common][timespan_t]")
{
    auto t = timespan_t::fromSeconds(1.2);

    REQUIRE(1.2 == t.totalSeconds());
    REQUIRE(1200.0 == t.totalMilliseconds());
}

TEST_CASE("Time_Can_Be_Created_In_Milliseconds", "[common][timespan_t]")
{
    auto t = timespan_t::fromMilliseconds(1500.0);

    REQUIRE(1.5 == t.totalSeconds());
    REQUIRE(1500.0 == t.totalMilliseconds());
}

TEST_CASE("Time_Can_Be_Created_As_A_User_Literal_In_Seconds", "[common][timespan_t]")
{
    auto t = 1.3_s;
    REQUIRE(1.3 == t.totalSeconds());
}

TEST_CASE("Time_Can_Be_Created_As_A_User_Literal_In_Milliseconds", "[common][timespan_t]")
{
    auto t = 1400.0_ms;
    REQUIRE(1.4 == t.totalSeconds());
}

TEST_CASE("Time_Can_Be_Copy_Constructed", "[common][timespan_t]")
{
    auto t = timespan_t::fromSeconds(1.4);
    timespan_t copy(t);

    REQUIRE(1.4 == copy.totalSeconds());
    REQUIRE(1.4 == t.totalSeconds());
}

TEST_CASE("Time_Can_Be_Assigned", "[common][timespan_t]")
{
    auto t = timespan_t::fromSeconds(1.5);
    auto copy = timespan_t::fromSeconds(0);

    REQUIRE(0.0 == copy.totalSeconds());

    copy = t;

    REQUIRE(1.5 == copy.totalSeconds());
    REQUIRE(1.5 == t.totalSeconds());
}

TEST_CASE("Time_Duration_Is_Always_Positive", "[common][timespan_t]")
{
    REQUIRE(0.0 == timespan_t::fromSeconds(0).duration().totalSeconds());
    REQUIRE(1.0 == timespan_t::fromSeconds(1).duration().totalSeconds());
    REQUIRE(2.0 == timespan_t::fromSeconds(-2).duration().totalSeconds());
}

TEST_CASE("Time_Swap_Swaps_Time", "[common][timespan_t]")
{
    auto a = timespan_t::fromSeconds(5.0);
    auto b = timespan_t::fromSeconds(6.0);

    swap(a, b);

    REQUIRE(6.0 == a.totalSeconds());
    REQUIRE(5.0 == b.totalSeconds());
}

TEST_CASE("Time_Can_Be_Reported_In_Seconds", "[common][timespan_t]")
{
    REQUIRE(1.2 == timespan_t::fromSeconds(1.2).duration().totalSeconds());
    REQUIRE(1.2 == timespan_t::fromMilliseconds(1200).duration().totalSeconds());
}

TEST_CASE("Time_Can_Be_Reported_In_Milliseconds", "[common][timespan_t]")
{
    REQUIRE(1200.0 == timespan_t::fromSeconds(1.2).duration().totalMilliseconds());
    REQUIRE(1200.0 == timespan_t::fromMilliseconds(1200).duration().totalMilliseconds());
}

TEST_CASE("Time_Can_Be_Negated", "[common][timespan_t]")
{
    REQUIRE(-1.2 == -timespan_t::fromSeconds(1.2).totalSeconds());
    REQUIRE(1.2 == -timespan_t::fromSeconds(-1.2).totalSeconds());
}

TEST_CASE("Time_Unary_Positive_Does_Nothing", "[common][timespan_t]")
{
    REQUIRE(1.2 == +timespan_t::fromSeconds(1.2).totalSeconds());
    REQUIRE(-1.2 == +timespan_t::fromSeconds(-1.2).totalSeconds());
}

TEST_CASE("Time_Can_Be_Added", "[common][timespan_t]")
{
    auto t = timespan_t::fromSeconds(2.0) + timespan_t::fromSeconds(0.8);
    REQUIRE(2.8 == t.totalSeconds());

    t += timespan_t::fromSeconds(0.2);
    REQUIRE(3.0 == t.totalSeconds());
}

TEST_CASE("Time_Can_Be_Subtracted", "[common][timespan_t]")
{
    auto t = timespan_t::fromSeconds(2.0) - timespan_t::fromSeconds(0.8);
    REQUIRE(1.2 == t.totalSeconds());

    t -= timespan_t::fromSeconds(0.2);
    REQUIRE(1.0 == t.totalSeconds());
}

TEST_CASE("Time_Can_Be_Multiplied", "[common][timespan_t]")
{
    auto t = timespan_t::fromSeconds(2.0) * 2.0;
    REQUIRE(4.0 == t.totalSeconds());

    t *= 3.0;
    REQUIRE(12.0 == t.totalSeconds());
}

TEST_CASE("Time_Can_Be_Divided", "[common][timespan_t]")
{
    auto t = timespan_t::fromSeconds(42.0) / 2.0;
    REQUIRE(21.0 == t.totalSeconds());

    t /= 3.0;
    REQUIRE(7.0 == t.totalSeconds());
}

TEST_CASE("Time_Can_Be_Checked_For_Equality", "[common][timespan_t]")
{
    auto a = timespan_t::fromSeconds(10.0);
    auto b = timespan_t::fromSeconds(20.0) / 2.0;
    auto c = timespan_t::fromSeconds(10.1);

    REQUIRE(a == a);
    REQUIRE(a == b);
    REQUIRE(b == a);
    REQUIRE(!(a == c));
}

TEST_CASE("Time_Can_Be_Checked_For_Inequality", "[common][timespan_t]")
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

TEST_CASE("Time_Can_Be_Compared", "[common][timespan_t]")
{
    auto a = timespan_t::fromSeconds(10.0);
    auto b = timespan_t::fromSeconds(25.0);
    auto c = timespan_t::fromSeconds(30.0);

    REQUIRE(a < b);
    REQUIRE(!(b < a));
    REQUIRE(a < c);
    REQUIRE(!(c < b));

    REQUIRE(a <= b);
    REQUIRE(a <= a);
    REQUIRE(!(b <= a));
       
    REQUIRE(b > a);
    REQUIRE(!(a > b));
    REQUIRE(c > b);
    REQUIRE(!(b > c));
    REQUIRE(c > a);
    REQUIRE(!(a > c));

    REQUIRE(b >= a);
    REQUIRE(b >= b);
    REQUIRE(c >= c);
    REQUIRE(!(a >= b));
}
