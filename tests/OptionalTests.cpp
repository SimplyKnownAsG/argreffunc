#include "arf/argreffunc.hpp"

#include "catch.hpp"

#include <iostream>

class SomeClass {
public:
    double member_dubs;
};

TEST_CASE("optional", "[optional]") {
    arf::Parser parser("prog");
    int int_val = 0;
    float float_val = 0.0f;

    SomeClass sc;
    std::function<void(double)> func = [&](double arg) -> void { sc.member_dubs = arg; };
    sc.member_dubs = 0.0;
    parser.add("first", "f", int_val);
    parser.add("second", "s", float_val);
    parser.add("third", "t", func);

    SECTION("optional 12") {
        int argc = 3;
        char* argv[] = { (char*)"program", (char*)"--first", (char*)"12" };

        parser.parse(argc, argv);

        REQUIRE(int_val == 12);
        REQUIRE(float_val == 0.0f);
        REQUIRE(sc.member_dubs == 0.0);
    }
    SECTION("optional=12") {
        std::vector<std::string> argv = { "--second=99.12" };

        parser.parse(argv);

        REQUIRE(int_val == 0);
        REQUIRE(float_val == 99.12f);
        REQUIRE(sc.member_dubs == 0.0);
    }
    SECTION("optional:12 fails") {
        std::vector<std::string> argv = { "--second:99.12" };

        REQUIRE_THROWS_AS(parser.parse(argv), arf::Exception);
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

TEST_CASE("multi-alias", "[optional]") {
    arf::Parser parser("prog");
    std::string thing = "";
    parser.add("thing1", "1", thing).add_alias("thing2").add_alias("t");
    std::vector<std::string> argv;

    SECTION("first alias") {
        argv.push_back("--thing1=bacon");
    }
    SECTION("second alias") {
        argv.push_back("--thing1=bacon");
    }
    SECTION("short alias no equal") {
        argv.push_back("-tbacon");
    }
    SECTION("short alias with equal") {
        argv.push_back("-t=bacon");
    }
    SECTION("short alias with space") {
        argv.push_back("-t");
        argv.push_back("bacon");
    }
    parser.parse(argv);
    REQUIRE(thing == "bacon");
}

TEST_CASE("stacking short options", "[optional]") {
    arf::Parser parser("prog");
    std::string thing = "";
    int verbosity = 0;
    parser.add("thing1", "1", thing).add_alias("t");
    parser.add("verbose", "1", [&verbosity]() { verbosity += 1; }).add_alias("v");

    SECTION("vtvalue") {
        std::vector<std::string> argv = { "-vtvalue" };
        parser.parse(argv);

        REQUIRE(verbosity == 1);
        REQUIRE(thing == "value");
    }
    SECTION("tv") {
        std::vector<std::string> argv = { "-tv" };
        parser.parse(argv);

        REQUIRE(verbosity == 0);
        REQUIRE(thing == "v");
    }
    SECTION("vv") {
        std::vector<std::string> argv = { "-vv" };
        parser.parse(argv);

        REQUIRE(verbosity == 2);
        REQUIRE(thing == "");
    }
    SECTION("vvt") {
        std::vector<std::string> argv = { "-vvtalue" };
        parser.parse(argv);

        REQUIRE(verbosity == 2);
        REQUIRE(thing == "alue");
    }
    SECTION("vt=val v") {
        std::vector<std::string> argv = { "-vtvalue", "-v" };
        parser.parse(argv);

        REQUIRE(verbosity == 2);
        REQUIRE(thing == "value");
    }
}
