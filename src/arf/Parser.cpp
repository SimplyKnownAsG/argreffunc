#include "arf/Parser.hpp"
#include "arf/Exception.hpp"

#include <iostream>

namespace arf {

    Parser::Parser(std::string program)
      : program(program) {}

    void Parser::parse(int argc, char* argv[]) {
        ArgIterator iterator(argc, argv);
        this->_parse(iterator);
    }

    void Parser::parse(std::vector<std::string> args) {
        ArgIterator iterator(args);
        this->_parse(iterator);
    }

    void Parser::print_help() {
        this->print_help(std::cout);
    }

    void Parser::print_help(std::ostream& stream) {
        stream << this->program << "\n";
        for (auto& arg : this->args) {
            arg->print_help(stream);
        }
    }

    void Parser::_parse(ArgIterator& iterator) {
        while (iterator.next()) {
            bool matched = false;

            for (auto arg : this->args) {
                matched = arg->parse(iterator);

                if (matched) {
                    break;
                }
            }

            if (!matched) {
                throw Exception("Argument was not consumed: " + iterator.current());
            }
        }

        auto num_positional =
          std::count_if(this->args.begin(), this->args.end(), [](Arg const* a) -> bool {
              return a->required && a->name.is_positional;
          });

        if (iterator.matched_positional_args.size() < num_positional) {
            std::ostringstream err_msg;
            err_msg << "too few positional arguments.";
            throw Exception(err_msg.str());
        }
    }
}
