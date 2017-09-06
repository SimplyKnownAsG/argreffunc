#include "argreffunc.hpp"

#include "catch.hpp"

#include <iostream>

class SomeClass {
public:
    double member_dubs;
};

TEST_CASE("positional", "[positional]") {
    arf::Parser parser;
    int int_val;
    float float_val = 0.0f;

    SomeClass sc;
    std::function<void(double)> func = [&](double arg) -> void { sc.member_dubs = arg; };
    sc.member_dubs = 0.0;
    parser.add_positional("first", int_val);
    parser.add_positional("second", float_val);
    parser.add_positional("third", func);

    SECTION("success with argc argv") {
        int argc = 4;
        char* argv[] = { "program", "12", "11.11", "99e99" };

        parser.parse(argc, argv);

        REQUIRE(int_val == 12);
        REQUIRE(float_val == 11.11f);
        REQUIRE(sc.member_dubs == 99e99);
    }
    SECTION("success with vector") {
        std::vector<std::string> args = { "4", "19.19", "99e99" };

        parser.parse(args);

        REQUIRE(int_val == 4);
        REQUIRE(float_val == 19.19f);
        REQUIRE(sc.member_dubs == 99e99);
    }
    SECTION("too few") {
        std::vector<std::string> args = { "7" };

        REQUIRE_THROWS_AS(parser.parse(args), arf::ArfException);

        REQUIRE(int_val == 7);
        REQUIRE(float_val == 0.0f);
        REQUIRE(sc.member_dubs == 0.0);
    }
    SECTION("too many") {
        std::vector<std::string> args = { "9", "12.12", "42e42", "barf" };

        REQUIRE_THROWS_AS(parser.parse(args), arf::ArfException);

        REQUIRE(int_val == 9);
        REQUIRE(float_val == 12.12f);
        REQUIRE(sc.member_dubs == 42e42);
    }
}
