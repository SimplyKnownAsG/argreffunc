#include "arf/argreffunc.hpp"

#include "catch.hpp"

#include <iostream>

using namespace Catch;

TEST_CASE("name collisions", "[errorhandling][!mayfail]") {
    arf::Parser parser("prog");
    std::vector<std::string> argv;

    SECTION("duplicate alias") {
        std::string name1;
        parser.add("name1", "desc 1", name1).add_alias("name1");
        REQUIRE_THROWS_AS(parser.parse(argv), arf::Exception);
    }
    SECTION("overlapping long names") {
        std::string name1, name2;
        parser.add("name", "desc 1", name1);
        parser.add("name", "desc 2", name2);
        REQUIRE_THROWS_AS(parser.parse(argv), arf::Exception);
    }
    SECTION("overlapping short names") {
        std::string name1, name2;
        parser.add("name1", "desc 1", name1).add_alias("n");
        parser.add("name2", "desc 2", name2).add_alias("n");
        REQUIRE_THROWS_AS(parser.parse(argv), arf::Exception);
    }
    SECTION("overlapping short and long names") {
        std::string name1, name2;
        parser.add("name1", "desc 1", name1).add_alias("n");
        parser.add("n", "desc 2", name2);
        REQUIRE_THROWS_AS(parser.parse(argv), arf::Exception);
    }
}

TEST_CASE("bad arguments", "[errorhandling]") {
    arf::Parser parser("prog");
    std::vector<std::string> argv;

    SECTION("missing value RefArg") {
        std::string name1;
        parser.add("name1", "desc 1", name1);
        argv.push_back("--name1");
        REQUIRE_THROWS_WITH(parser.parse(argv), Contains("missing value"));
    }
    SECTION("missing value FuncArg") {
        bool called;
        parser.add<std::string>(
                "name1", "desc 1", [&](std::string nope) -> void { called = true; });
        argv.push_back("--name1");
        REQUIRE_THROWS_WITH(parser.parse(argv), Contains("missing value"));
    }
}
