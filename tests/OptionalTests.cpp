#include "argreffunc.hpp"

#include "catch.hpp"

#include <iostream>

class SomeClass {
public:
    double member_dubs;
};

TEST_CASE("optional", "[optional]") {
    arf::Parser parser;
    int int_val = 0;
    float float_val = 0.0f;

    SomeClass sc;
    std::function<void(double)> func = [&](double arg) -> void { sc.member_dubs = arg; };
    sc.member_dubs = 0.0;
    parser.add("first", int_val);
    parser.add("second", float_val);
    parser.add("third", func);

    SECTION("positional 12") {
        int argc = 3;
        char* argv[] = { "program", "--first", "12" };

        parser.parse(argc, argv);

        REQUIRE(int_val == 12);
        REQUIRE(float_val == 0.0f);
        REQUIRE(sc.member_dubs == 0.0);
    }
    SECTION("positional=12") {
        std::vector<std::string> argv = { "--second=99.12" };

        parser.parse(argv);

        REQUIRE(int_val == 0);
        REQUIRE(float_val == 99.12f);
        REQUIRE(sc.member_dubs == 0.0);
    }
    SECTION("positional:12 fails") {
        std::vector<std::string> argv = { "--second:99.12" };

        REQUIRE_THROWS_AS(parser.parse(argv), arf::ArfException);
        REQUIRE(float_val == 0.0f);
    }
    SECTION("multiple args") {
        std::vector<std::string> args = { "--third=99e99", "--first", "4", "--second=19.19" };

        parser.parse(args);

        REQUIRE(int_val == 4);
        REQUIRE(float_val == 19.19f);
        REQUIRE(sc.member_dubs == 99e99);
    }
}
