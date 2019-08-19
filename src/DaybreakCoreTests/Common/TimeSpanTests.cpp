#include "stdafx.h"
#include "CppUnitTest.h"

#include "app/time.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

TEST_CLASS(TimeSpanTests)
{
public:
    TEST_METHOD(Time_Can_Be_Created_In_Seconds)
    {
        auto t = timespan_t::fromSeconds(1.2);

        Assert::AreEqual(1.2, t.totalSeconds());
        Assert::AreEqual(1200.0, t.totalMilliseconds());
    }

    TEST_METHOD(Time_Can_Be_Created_In_Milliseconds)
    {
        auto t = timespan_t::fromMilliseconds(1500.0);

        Assert::AreEqual(1.5, t.totalSeconds());
        Assert::AreEqual(1500.0, t.totalMilliseconds());
    }

    TEST_METHOD(Time_Can_Be_Created_As_A_User_Literal_In_Seconds)
    {
        auto t = 1.3_s;
        Assert::AreEqual(1.3, t.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Created_As_A_User_Literal_In_Milliseconds)
    {
        auto t = 1400.0_ms;
        Assert::AreEqual(1.4, t.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Copy_Constructed)
    {
        auto t = timespan_t::fromSeconds(1.4);
        timespan_t copy(t);

        Assert::AreEqual(1.4, copy.totalSeconds());
        Assert::AreEqual(1.4, t.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Assigned)
    {
        auto t = timespan_t::fromSeconds(1.5);
        auto copy = timespan_t::fromSeconds(0);

        Assert::AreEqual(0.0, copy.totalSeconds());

        copy = t;

        Assert::AreEqual(1.5, copy.totalSeconds());
        Assert::AreEqual(1.5, t.totalSeconds());
    }

    TEST_METHOD(Time_Duration_Is_Always_Positive)
    {
        Assert::AreEqual(0.0, timespan_t::fromSeconds(0).duration().totalSeconds());
        Assert::AreEqual(1.0, timespan_t::fromSeconds(1).duration().totalSeconds());
        Assert::AreEqual(2.0, timespan_t::fromSeconds(-2).duration().totalSeconds());
    }

    TEST_METHOD(Time_Swap_Swaps_Time)
    {
        auto a = timespan_t::fromSeconds(5.0);
        auto b = timespan_t::fromSeconds(6.0);

        swap(a, b);

        Assert::AreEqual(6.0, a.totalSeconds());
        Assert::AreEqual(5.0, b.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Reported_In_Seconds)
    {
        Assert::AreEqual(1.2, timespan_t::fromSeconds(1.2).duration().totalSeconds());
        Assert::AreEqual(1.2, timespan_t::fromMilliseconds(1200).duration().totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Reported_In_Milliseconds)
    {
        Assert::AreEqual(1200.0, timespan_t::fromSeconds(1.2).duration().totalMilliseconds());
        Assert::AreEqual(1200.0, timespan_t::fromMilliseconds(1200).duration().totalMilliseconds());
    }

    TEST_METHOD(Time_Can_Be_Negated)
    {
        Assert::AreEqual(-1.2, -timespan_t::fromSeconds(1.2).totalSeconds());
        Assert::AreEqual(1.2, -timespan_t::fromSeconds(-1.2).totalSeconds());
    }

    TEST_METHOD(Time_Unary_Positive_Does_Nothing)
    {
        Assert::AreEqual(1.2, +timespan_t::fromSeconds(1.2).totalSeconds());
        Assert::AreEqual(-1.2, +timespan_t::fromSeconds(-1.2).totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Added)
    {
        auto t = timespan_t::fromSeconds(2.0) + timespan_t::fromSeconds(0.8);
        Assert::AreEqual(2.8, t.totalSeconds());

        t += timespan_t::fromSeconds(0.2);
        Assert::AreEqual(3.0, t.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Subtracted)
    {
        auto t = timespan_t::fromSeconds(2.0) - timespan_t::fromSeconds(0.8);
        Assert::AreEqual(1.2, t.totalSeconds());

        t -= timespan_t::fromSeconds(0.2);
        Assert::AreEqual(1.0, t.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Multiplied)
    {
        auto t = timespan_t::fromSeconds(2.0) * 2.0;
        Assert::AreEqual(4.0, t.totalSeconds());

        t *= 3.0;
        Assert::AreEqual(12.0, t.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Divided)
    {
        auto t = timespan_t::fromSeconds(42.0) / 2.0;
        Assert::AreEqual(21.0, t.totalSeconds());

        t /= 3.0;
        Assert::AreEqual(7.0, t.totalSeconds());
    }

    TEST_METHOD(Time_Can_Be_Checked_For_Equality)
    {
        auto a = timespan_t::fromSeconds(10.0);
        auto b = timespan_t::fromSeconds(20.0) / 2.0;
        auto c = timespan_t::fromSeconds(10.1);

        Assert::IsTrue(a == a);
        Assert::IsTrue(a == b);
        Assert::IsTrue(b == a);
        Assert::IsFalse(a == c);
    }

    TEST_METHOD(Time_Can_Be_Checked_For_Inequality)
    {
        auto a = timespan_t::fromSeconds(10.0);
        auto b = timespan_t::fromSeconds(20.0) / 2.0;
        auto c = timespan_t::fromSeconds(10.1);

        Assert::IsTrue(a != c);
        Assert::IsTrue(c != a);

        Assert::IsFalse(a != a);
        Assert::IsFalse(a != b);
        Assert::IsFalse(b != a);
    }

    TEST_METHOD(Time_Can_Be_Compared)
    {
        auto a = timespan_t::fromSeconds(10.0);
        auto b = timespan_t::fromSeconds(25.0);
        auto c = timespan_t::fromSeconds(30.0);

        Assert::IsTrue(a < b);
        Assert::IsFalse(b < a);
        Assert::IsTrue(a < c);
        Assert::IsFalse(c < b);

        Assert::IsTrue(a <= b);
        Assert::IsTrue(a <= a);
        Assert::IsFalse(b <= a);
       
        Assert::IsTrue(b > a);
        Assert::IsFalse(a > b);
        Assert::IsTrue(c > b);
        Assert::IsFalse(b > c);
        Assert::IsTrue(c > a);
        Assert::IsFalse(a > c);

        Assert::IsTrue(b >= a);
        Assert::IsTrue(b >= b);
        Assert::IsTrue(c >= c);
        Assert::IsFalse(a >= b);
    }
};