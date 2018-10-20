#include "arf/Arg.hpp"
#include "arf/Exception.hpp"

namespace arf {

    Arg::Arg(std::string name, std::string help)
      : help(help) {
        this->names.push_back(name);
    }

    void Arg::parse(std::istringstream& stream) {
        this->parse_hook(stream);
        if (stream.bad()) {
            std::ostringstream err_msg;
            err_msg << "Failed to parse `" << this->names[0] << "`, stream state is bad.";
            throw Exception(err_msg.str());
        }
        if (!stream.eof()) {
            std::ostringstream err_msg;
            err_msg << "Failed to parse `" << this->names[0]
                    << "`, not at end of stream: " << stream.str();
            throw Exception(err_msg.str());
        }
    }

    Arg& Arg::add_alias(std::string alias) {
        if (alias.size() == 1) {
            this->aliases.push_back(alias);
        } else {
            this->names.push_back(alias);
        }
        return *this;
    }

    void Arg::print_help(std::ostream& stream) const {
        auto write_names = [&stream](std::string prefix, std::vector<std::string> names) -> void {
            // start with new line, this prevents a blank line after a program
            // without any arguments. it also removes need for extra logic to
            // prevent new line between this and the help description.
            if (names.size() > 0) {
                stream << "\n";
            }
            for (auto n = 0; n < names.size(); ++n) {
                auto name = names[n];

                if (n == 0) {
                    stream << "    ";
                } else {
                    stream << ", ";
                }

                stream << prefix << name;

                if (n == 0) {
                    stream << " <" << name << ">";
                }
            }
        };

        write_names("--", this->names);
        write_names("-", this->aliases);

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
}
