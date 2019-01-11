#include "arf/argreffunc.hpp"

#include "catch.hpp"

#include <iostream>

using namespace arf;

class SomeClass {
public:
    double member_dubs;
};

TEST_CASE("positional", "[positional]") {
    Parser parser("prog");
    int int_val;
    float float_val = 0.0f;

    SomeClass sc;
    std::function<void(double)> func = [&](double arg) -> void { sc.member_dubs = arg; };
    sc.member_dubs = 0.0;
    parser.add_positional("first", "f", int_val);
    parser.add_positional("second", "s", float_val);
    parser.add_positional("third", "t", func);

    SECTION("success with argc argv") {
        int argc = 4;
        char* argv[] = { (char*)"program", (char*)"12", (char*)"11.11", (char*)"99e99" };

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
        REQUIRE_THROWS_AS(parser.parse(args), Exception);
    }
    SECTION("too many") {
        std::vector<std::string> args = { "9", "12.12", "42e42", "barf" };

        REQUIRE_THROWS_AS(parser.parse(args), Exception);
    }
}

TEST_CASE("positional multile-values", "[positional][multi]") {
    Parser parser("prog");

    SECTION("multiple values") {
        std::vector<std::string> values;
        std::vector<std::string> args = { "This", "is", "a", "sentence." };
        std::function<void(ArgIterator&)> func = [&](ArgIterator& iterator) {
            iterator.get_values("values", values);
        };
        parser.add_positional("values", "all values", func);
        parser.parse(args);
        REQUIRE(values.size() == args.size());
        REQUIRE(values.at(2) == "a");
    }
}
