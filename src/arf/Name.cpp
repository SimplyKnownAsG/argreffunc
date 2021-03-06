#include "arf/Name.hpp"
#include "arf/ArgIterator.hpp"
#include <iostream>
namespace arf {
    Name::Name(std::string name)
      : name(name) {
        this->names.push_back(name);
    }

    Name& Name::add_alias(std::string alias) {
        if (alias.size() == 1) {
            this->aliases.push_back(alias);
        } else {
            this->names.push_back(alias);
        }
        return *this;
    }

    void Name::print_help(std::ostream& stream,
                          bool is_positional,
                          bool is_required,
                          bool is_switch) const {
        auto write_names = [&](std::string prefix, std::vector<std::string> names) -> void {
            // start with new line, this prevents a blank line after a program
            // without any arguments. it also removes need for extra logic to
            // prevent new line between this and the help description.
            if (names.size() > 0) {
                stream << "\n";
            }

            for (auto ni = 0; ni < names.size(); ++ni) {
                auto n = names[ni];

                if (ni == 0) {
                    stream << "    ";
                } else {
                    stream << ", ";
                }

                if (is_positional) {
                    if (is_required) {
                        stream << "<" << n << ">";
                    } else {
                        stream << "[" << n << "]";
                    }
                } else {
                    stream << prefix << n;
                    if (!is_switch) {
                        stream << " <" << this->name << ">";
                    }
                }
            }
        };

        write_names("--", this->names);
        write_names("-", this->aliases);
    }
}
