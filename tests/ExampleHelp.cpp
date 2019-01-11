#include "arf/argreffunc.hpp"

#include "catch.hpp"

#include <iostream>
#include <sstream>

TEST_CASE("example-help", "[example]") {
    arf::Parser parser("the-program-name");

    SECTION("adding a help option") {
        bool show_help = false;
        parser.add("help", "show help", [&show_help](void) { show_help = true; })
                .add_alias("h")
                .add_alias("?");
        std::vector<std::string> params = { "--help" };
        parser.parse(params);
        REQUIRE(show_help == true);
    }

    /// this is a comment, that I would like broken out
    SECTION("really long documentation") {
        std::string greeting = "Hello";
        std::string name = "World";
        parser.add(
                "name",
                R"(Lorem ipsum dolor sit amet, inani oratio salutandi ne mea, qui in affert nostro
              officiis, te pri velit cetero. Nibh expetenda consequuntur ex vim. Dictas civibus mei
              no. An iudico noster eam, eu labore accusam molestiae pro. Minim libris id mei, ex
              vide semper vix. Te ancillae molestie officiis qui, no ancillae reprimique est. Qui
              affert philosophia at, sit in mollis aliquam eloquentiam.

              Cum cu nobis oratio cotidieque, mel id dicta accusam efficiendi, pri ubique appetere
              at. Ad ubique aperiri scribentur nec, mea ei latine molestie. Eos iisque suscipiantur
              eu. Sit ad option docendi conceptam. Vis timeam adversarium in. At pro exerci
              propriae, usu id brute augue dicant, vim aliquando eloquentiam in.

              Quo an dolores gloriatur, vel delenit graecis eloquentiam et. Quo vivendo intellegebat
              te, alii postea utamur pri ea. Mel eu iuvaret deserunt, usu facete honestatis id, ne
              quo quot summo.  Vis ne aperiri rationibus. Enim mollis tincidunt ex eos, eu eam
              nostro electram cotidieque.)",
                name);
        parser.add("greeting", "the thing ur gonna say", greeting);
        std::stringstream stream;
        parser.print_help(stream);
        auto expected =
                R"(the-program-name

    --name <name>
        Lorem ipsum dolor sit amet, inani oratio salutandi ne mea, qui in affert
        nostro officiis, te pri velit cetero. Nibh expetenda consequuntur ex
        vim. Dictas civibus mei no. An iudico noster eam, eu labore accusam
        molestiae pro. Minim libris id mei, ex vide semper vix. Te ancillae
        molestie officiis qui, no ancillae reprimique est. Qui affert
        philosophia at, sit in mollis aliquam eloquentiam.

        Cum cu nobis oratio cotidieque, mel id dicta accusam efficiendi, pri
        ubique appetere at. Ad ubique aperiri scribentur nec, mea ei latine
        molestie. Eos iisque suscipiantur eu. Sit ad option docendi conceptam.
        Vis timeam adversarium in. At pro exerci propriae, usu id brute augue
        dicant, vim aliquando eloquentiam in.

        Quo an dolores gloriatur, vel delenit graecis eloquentiam et. Quo
        vivendo intellegebat te, alii postea utamur pri ea. Mel eu iuvaret
        deserunt, usu facete honestatis id, ne quo quot summo. Vis ne aperiri
        rationibus. Enim mollis tincidunt ex eos, eu eam nostro electram
        cotidieque.

    --greeting <greeting>
        the thing ur gonna say
)";
        REQUIRE(expected == stream.str());
    }
    SECTION("double spaces removed") {
        std::string spaced = "...";
        parser.add("spaced", " there is a  double.  space", spaced);
        std::string expected = R"(the-program-name

    --spaced <spaced>
        there is a double. space
)";
        std::stringstream stream;
        parser.print_help(stream);
        REQUIRE(expected == stream.str());
    }
    SECTION("long word extends beyond 80") {
        std::string spaced = "...";
        parser.add("spaced",
                   "first "
                   "its-not-that-i-don't-like-spaces,it's-just-that-i-think-there-is-another-way "
                   "last",
                   spaced);
        std::string expected = R"(the-program-name

    --spaced <spaced>
        first
        its-not-that-i-don't-like-spaces,it's-just-that-i-think-there-is-another-way
        last
)";
        std::stringstream stream;
        parser.print_help(stream);
        REQUIRE(expected == stream.str());
    }
    SECTION("multiple names and aliases") {
        std::string spaced = "...";
        parser.add("name1", "description of name1", spaced)
                .add_alias("name2")
                .add_alias("n")
                .add_alias("1")
                .add_alias("2");
        std::string expected = R"(the-program-name

    --name1 <name1>, --name2 <name1>
    -n <name1>, -1 <name1>, -2 <name1>
        description of name1
)";
        std::stringstream stream;
        parser.print_help(stream);
        REQUIRE(expected == stream.str());
    }
    SECTION("required positional") {
        std::string arg_ref = "...";
        parser.add_positional("required-positional", "description of arg", arg_ref);
        std::string expected = R"(the-program-name

    <required-positional>
        description of arg
)";
        std::stringstream stream;
        parser.print_help(stream);
        REQUIRE(expected == stream.str());
    }
    SECTION("optional positional") {
        std::string arg_ref = "...";
        parser.add_positional("optional-positional", "description of arg", arg_ref, false);
        std::string expected = R"(the-program-name

    [optional-positional]
        description of arg
)";
        std::stringstream stream;
        parser.print_help(stream);
        REQUIRE(expected == stream.str());
    }
    SECTION("switch") {
        bool arg_ref = false;
        parser.add("switch", "description of arg", [&]() { arg_ref = true; });
        std::string expected = R"(the-program-name

    --switch
        description of arg
)";
        std::stringstream stream;
        parser.print_help(stream);
        REQUIRE(expected == stream.str());
    }
}
