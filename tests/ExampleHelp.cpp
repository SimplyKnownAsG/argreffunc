#include "arf/argreffunc.hpp"

#include "catch.hpp"

#include <iostream>
#include <sstream>

TEST_CASE("example-help", "[example]") {
    arf::Parser parser("the-program-name");

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
        parser.add(
          "spaced",
          "first its-not-that-i-don't-like-spaces,it's-just-that-i-think-there-is-another-way last",
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

    --name1 <name1>, --name2
    -n <n>, -1, -2
        description of name1
)";
        std::stringstream stream;
        parser.print_help(stream);
        REQUIRE(expected == stream.str());
    }
}
