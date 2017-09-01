#include "argreffunc.hpp"

#include "catch.hpp"

#include <iostream>

class SomeClass {
public:
    double member_dubs;
};

TEST_CASE("all", "[all]") {
    int argc = 4;
    char* argv[] = { "program", "12", "11.11", "99e99" };
    arf::Parser parser;
    int int_val;
    float float_val;

    SomeClass sc;
    std::function<void(double)> func = [&](double arg) -> void {
        std::cout << "dubbin" << std::endl;
        sc.member_dubs = arg;
    };
    parser.add("first", int_val).add("second", float_val).add("third", func);

    parser.parse(argc, argv);

    REQUIRE(int_val == 12);
    REQUIRE(float_val == 11.11f);
    REQUIRE(sc.member_dubs == 99e99);
}
