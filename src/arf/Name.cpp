#include "arf/Name.hpp"
#include "arf/ArgIterator.hpp"
#include <iostream>
namespace arf {
    Name::Name(std::string name, bool is_positional)
      : name(name)
      , is_positional(is_positional) {
        this->names.push_back(name);
    }

    bool Name::matches(ArgIterator& iterator) {
        if (iterator.type() == ArgType::Positional) {
            if (this->is_positional) {
                if (iterator.matched_positional_args.count(this)) {
                    return false;
                }

                iterator.matched_positional_args.insert(this);
                return true;
            }
            return false;
        }

        auto& names = iterator.type() == ArgType::Long ? this->names : this->aliases;
        size_t start = iterator.type() == ArgType::Long ? 2 : 1;

        for (const auto& name : names) {
            if (iterator.current().find(name) == start) {
                auto value_start = name.size() + start;

                if (iterator.type() == ArgType::Long) {
                    if (iterator.current().size() > name.size() + 2 &&
                        iterator.current()[name.size() + 2] != '=') {
                        std::ostringstream err_msg;
                        err_msg << "expected `=` sign after argument `" << name << "`, but got: `"
                                << iterator.current() << "`.";
                        throw Exception(err_msg.str());
                    }
                    value_start += 1;
                } else /* if (iterator.type() == ArgType::Long) */ {
                    if (iterator.current().size() > name.size() + 1 &&
                        iterator.current()[name.size() + 1] == '=') {
                        value_start += 1;
                    }
                }

                iterator.value_start = value_start;
                return true;
            }
        }

        return false;
    }

    Name& Name::add_alias(std::string alias) {
        if (alias.size() == 1) {
            this->aliases.push_back(alias);
        } else {
            this->names.push_back(alias);
        }
        return *this;
    }

    void Name::print_help(std::ostream& stream) const {
        auto write_names = [&stream, this](std::string prefix,
                                           std::vector<std::string> names) -> void {
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

                stream << prefix << n;

                if (ni == 0) {
                    stream << " <" << this->name << ">";
                }
            }
        };

        write_names("--", this->names);
        write_names("-", this->aliases);
    }
}
