#include "arf/Parser.hpp"
#include "arf/Exception.hpp"

#include <iostream>
#include <unordered_map>

using std::string;
using std::tuple;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace arf {

    Parser::Parser(string program)
      : program(program) {}

    void Parser::parse(int argc, char* argv[]) {
        ArgIterator iterator(argc, argv);
        this->_parse(iterator);
    }

    void Parser::parse(vector<string> args) {
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

    tuple<unordered_map<string, Arg*>, vector<Arg*>> categorize_args(vector<unique_ptr<Arg>>& args);

    void Parser::_parse(ArgIterator& iterator) {
        auto categorized = categorize_args(this->args);
        auto named = std::get<0>(categorized);
        auto positional = std::get<1>(categorized);
        auto piter = positional.begin();

        while (iterator.next()) {
            switch (iterator.type()) {
            case ArgType::Long:
            case ArgType::Short: {
                auto name = iterator.get_name();
                if (!named.count(name)) {
                    std::ostringstream err_msg;
                    err_msg << "Urecognized argument `" << iterator.current() << "`."
                            << " Note that --named arguments must use an = as a value separator.";
                    throw Exception(err_msg.str());
                }
                auto arg = named.at(name);
                arg->parse(iterator);
                break;
            }
            case ArgType::Positional: {
                if (piter == positional.end()) {
                    std::ostringstream err_msg;
                    err_msg << "Too many positional arguments at `" << iterator.current() << "`.";
                    throw Exception(err_msg.str());
                }
                auto arg = *piter;
                arg->parse(iterator);
                piter++;
                break;
            }
            default:
                throw Exception("This should not happen.");
            }
        }

        if (piter != positional.end()) {
            std::ostringstream err_msg;
            err_msg << "too few positional arguments.";
            throw Exception(err_msg.str());
        }
    }

    tuple<unordered_map<string, Arg*>, vector<Arg*>> categorize_args(
            vector<unique_ptr<Arg>>& args) {
        unordered_map<string, Arg*> named;
        vector<Arg*> positional;

        for (auto const& arg : args) {
            if (arg->is_positional) {
                positional.push_back(arg.get());
            } else {
                for (string const& name : arg->name.names) {
                    named[name] = arg.get();
                }
                for (string const& name : arg->name.aliases) {
                    named[name] = arg.get();
                }
            }
        }

        return tuple<unordered_map<string, Arg*>, vector<Arg*>>(named, positional);
    }
}
