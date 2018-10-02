#include "arf/argreffunc.hpp"

#include "catch.hpp"

#include <iostream>

TEST_CASE("example", "[example]") {
    arf::Parser parser;

    std::string name = "you there";
    parser.add("name", name).add_alias("n");

    int age;
    parser.add("age", age).add_alias("a");

    std::string domain;
    int port = 80;
    parser.add<std::string>("url", [&](std::string url) -> void {
        auto colon_loc = url.find(':');
        domain = url.substr(0, colon_loc);
        if (colon_loc != std::string::npos) {
            auto substring = url.substr(colon_loc + 1, url.size() - colon_loc - 1);
            port = std::stoi(substring);
        }
    });

    SECTION("example") {
        std::vector<std::string> args = { "--url", "site.com:1234" };
        parser.parse(args);
        REQUIRE(domain == "site.com");
        REQUIRE(port == 1234);
    }
}
