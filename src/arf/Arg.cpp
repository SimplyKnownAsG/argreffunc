#include "arf/Arg.hpp"
#include "arf/ArgIterator.hpp"
#include "arf/Exception.hpp"
#include "arf/SwitchArg.hpp"

namespace arf {

    Arg::Arg(std::string name, bool positional, bool required, std::string help)
      : name(name)
      , is_positional(positional)
      , required(required)
      , help(help) {}

    void Arg::parse(ArgIterator& iterator) {
        this->parse_hook(iterator);
    }

    void Arg::print_help(std::ostream& stream) const {
        this->name.print_help(stream,
                              this->is_positional,
                              this->required,
                              dynamic_cast<const SwitchArg*>(this) != nullptr);

        std::istringstream line_finder(help);
        std::string line;
        int linelength = 80; // force newline

        while (std::getline(line_finder, line)) {
            std::istringstream word_finder(line);
            std::string word;
            bool is_blank = true;

            while (std::getline(word_finder, word, ' ')) {
                if (word.size() == 0) {
                    continue;
                }

                is_blank = false;

                if (linelength + word.size() + 1 > 80) {
                    stream << "\n       ";
                    linelength = 7;
                }

                stream << " " << word;
                linelength += word.size() + 1;
            }

            if (is_blank) {
                stream << "\n";
                linelength = 80;
            }
        }

        stream << "\n";
    }

    Arg& Arg::add_alias(std::string alias) {
        this->name.add_alias(alias);
        return *this;
    }
}
