#include "stdafx.h"
#include "app/support/deref.h"

#include "../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("deref can cast non-null pointers to references", "[app][support][deref]")
{
    SECTION("raw pointers can be converted")
    {
        int* pFoo = new int(42);

        static_assert(std::is_same<decltype(deref(pFoo)), int&>());

        auto& foo = deref(pFoo);
        REQUIRE(foo == 42);

        delete pFoo;
    }

    SECTION("const raw pointers can be converted")
    {
        const int* pFoo = new int(22);

        static_assert(std::is_same<decltype(deref(pFoo)), const int&>());

        auto& foo = deref(pFoo);
        REQUIRE(foo == 22);

        delete pFoo;
    }

    SECTION("unique pointers can be converted")
    {
        std::unique_ptr<int> pFoo(new int(12));

        static_assert(std::is_same<decltype(deref(pFoo)), int&>());

        auto& foo = deref(pFoo);
        REQUIRE(foo == 12);
    }

    SECTION("unique pointers to constant data can be converted")
    {
        std::unique_ptr<const int> pFoo(new int(13));

        static_assert(std::is_same<decltype(deref(pFoo)), const int&>());

        auto& foo = deref(pFoo);
        REQUIRE(foo == 13);
    }

    SECTION("shared pointers can be converted")
    {
        std::shared_ptr<int> pFoo(new int(100));

        static_assert(std::is_same<decltype(deref(pFoo)), int&>());

        auto& foo = deref(pFoo);
        REQUIRE(foo == 100);
    }

    SECTION("shared pointers to constant data can be converted")
    {
        std::shared_ptr<const int> pFoo(new int(8));

        static_assert(std::is_same<decltype(deref(pFoo)), const int&>());

        auto& foo = deref(pFoo);
        REQUIRE(foo == 8);
    }
}

TEST_CASE("deref_checked can cast any pointer to a reference", "[app][support][deref][deref_checked]")
{
    SECTION("raw pointers can be converted")
    {
        int* pFoo = new int(42);

        static_assert(std::is_same<decltype(deref_checked(pFoo)), int&>());

        auto& foo = deref_checked(pFoo);
        REQUIRE(foo == 42);

        delete pFoo;
    }

    SECTION("null raw pointers throw an exception")
    {
        int* pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_checked(pFoo)), int&>());

        REQUIRE_THROWS_WITH(
            [pFoo] { deref_checked(pFoo); }(),
            Catch::Contains("Cannot convert null pointer to reference"));

        delete pFoo;
    }

    SECTION("const raw pointers can be converted")
    {
        const int* pFoo = new int(22);

        static_assert(std::is_same<decltype(deref_checked(pFoo)), const int&>());

        auto& foo = deref_checked(pFoo);
        REQUIRE(foo == 22);

        delete pFoo;
    }

    SECTION("const null raw pointers throw an exception")
    {
        const int* pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_checked(pFoo)), const int&>());

        REQUIRE_THROWS_WITH(
            [pFoo] { deref_checked(pFoo); }(),
            Catch::Contains("Cannot convert null pointer to reference"));

        delete pFoo;
    }

    SECTION("unique pointers can be converted")
    {
        std::unique_ptr<int> pFoo(new int(12));

        static_assert(std::is_same<decltype(deref_checked(pFoo)), int&>());

        auto& foo = deref_checked(pFoo);
        REQUIRE(foo == 12);
    }

    SECTION("null unique pointers throw an exception")
    {
        std::unique_ptr<int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_checked(pFoo)), int&>());

        REQUIRE_THROWS_WITH(
            [&pFoo] { deref_checked(pFoo); }(),
            Catch::Contains("Cannot convert null pointer to reference"));
    }

    SECTION("unique pointers to constant data can be converted")
    {
        std::unique_ptr<const int> pFoo(new int(13));

        static_assert(std::is_same<decltype(deref_checked(pFoo)), const int&>());

        auto& foo = deref_checked(pFoo);
        REQUIRE(foo == 13);
    }

    SECTION("null unique pointers to constant data throw an exception")
    {
        std::unique_ptr<const int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_checked(pFoo)), const int&>());

        REQUIRE_THROWS_WITH(
            [&pFoo] { deref_checked(pFoo); }(),
            Catch::Contains("Cannot convert null pointer to reference"));
    }

    SECTION("shared pointers can be converted")
    {
        std::shared_ptr<int> pFoo(new int(100));

        static_assert(std::is_same<decltype(deref_checked(pFoo)), int&>());

        auto& foo = deref_checked(pFoo);
        REQUIRE(foo == 100);
    }

    SECTION("null shared pointers throw an exception")
    {
        std::shared_ptr<int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_checked(pFoo)), int&>());

        REQUIRE_THROWS_WITH(
            [&pFoo] { deref_checked(pFoo); }(),
            Catch::Contains("Cannot convert null pointer to reference"));
    }

    SECTION("shared pointers to constant data can be converted")
    {
        std::shared_ptr<const int> pFoo(new int(8));

        static_assert(std::is_same<decltype(deref_checked(pFoo)), const int&>());

        auto& foo = deref_checked(pFoo);
        REQUIRE(foo == 8);
    }

    SECTION("null shared pointers to constant data throw an exception")
    {
        std::shared_ptr<const int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_checked(pFoo)), const int&>());

        REQUIRE_THROWS_WITH(
            [&pFoo] { deref_checked(pFoo); }(),
            Catch::Contains("Cannot convert null pointer to reference"));
    }
}

TEST_CASE("deref_unchecked can cast any pointer to a reference", "[app][support][deref][deref_unchecked]")
{
    SECTION("raw pointers can be converted")
    {
        int* pFoo = new int(42);

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(foo == 42);

        delete pFoo;
    }

    SECTION("null raw pointers can be converted")
    {
        int* pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(&foo == nullptr);

        delete pFoo;
    }

    SECTION("const raw pointers can be converted")
    {
        const int* pFoo = new int(22);

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), const int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(foo == 22);

        delete pFoo;
    }

    SECTION("const null raw pointers can be converted")
    {
        const int* pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), const int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(&foo == nullptr);

        delete pFoo;
    }

    SECTION("unique pointers can be converted")
    {
        std::unique_ptr<int> pFoo(new int(12));

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(foo == 12);
    }

    SECTION("null unique pointers can be converted")
    {
        std::unique_ptr<int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(&foo == nullptr);
    }

    SECTION("unique pointers to constant data can be converted")
    {
        std::unique_ptr<const int> pFoo(new int(13));

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), const int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(foo == 13);
    }

    SECTION("null unique pointers to constant data can be converted")
    {
        std::unique_ptr<const int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), const int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(&foo == nullptr);
    }

    SECTION("shared pointers can be converted")
    {
        std::shared_ptr<int> pFoo(new int(100));

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(foo == 100);
    }

    SECTION("null shared pointers can be converted")
    {
        std::shared_ptr<int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(&foo == nullptr);
    }

    SECTION("shared pointers to constant data can be converted")
    {
        std::shared_ptr<const int> pFoo(new int(8));

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), const int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(foo == 8);
    }

    SECTION("null shared pointers to constant data can be converted")
    {
        std::shared_ptr<const int> pFoo = nullptr;

        static_assert(std::is_same<decltype(deref_unchecked(pFoo)), const int&>());

        auto& foo = deref_unchecked(pFoo);
        REQUIRE(&foo == nullptr);
    }
}
