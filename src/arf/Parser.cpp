#include "arf/Parser.hpp"
#include "arf/Exception.hpp"

#include <iostream>

namespace arf {

    Parser::Parser(std::string program)
      : program(program) {}

    void Parser::_parse(ArgIterator& iterator, std::vector<Arg*>::iterator& positional) {
        while (iterator.next()) {
            if (iterator.current()[0] == '-') {
                if (iterator.current()[1] == '-') {
                    this->_parse_long(iterator);
                } else {
                    this->_parse_short(iterator);
                }
            } else {
                this->_parse_positional(iterator, positional);
            }
        }

        if (positional != this->positional_args.end()) {
            std::ostringstream err_msg;
            err_msg << "too few positional arguments.";
            throw Exception(err_msg.str());
        }
    }

    void Parser::_parse_long(ArgIterator& iterator) {
        bool success = false;
        for (auto a : this->args) {
            /* 2 after leading "--" */
            for (const auto& name : a->names) {
                if (iterator.current().find(name) == 2) {
                    success = true;
                    std::istringstream stream;

                    if (iterator.current().size() == name.size() + 2) {
                        iterator.next();
                        stream.str(iterator.current());
                    } else if (iterator.current().size() > name.size() + 2) {
                        if (iterator.current()[name.size() + 2] != '=') {
                            std::ostringstream err_msg;
                            err_msg << "expected `=` sign after argument `" << name
                                    << "`, but got:" << iterator.current() << "`.";
                            throw Exception(err_msg.str());
                        }
                        stream.str(iterator.current().substr(3 + name.size()));
                    } else {
                        std::ostringstream err_msg;
                        err_msg << "I don't understand how this happened.";
                        throw Exception(err_msg.str());
                    }

                    a->parse(stream);
                    break;
                }
            }

            if (success) {
                break;
            }
        }

        if (!success) {
            std::ostringstream err_msg;
            err_msg << "unrecognized argument: `" << iterator.current() << "`.";
            throw Exception(err_msg.str());
        }
    }

    void Parser::_parse_short(ArgIterator& iterator) {
        bool success = false;
        for (auto a : this->args) {
            /* 1 after leading "-" */
            for (const auto& name : a->aliases) {
                if (iterator.current().find(name) == 1) {
                    success = true;
                    std::istringstream stream;

                    if (iterator.current().size() == name.size() + 1) {
                        iterator.next();
                        stream.str(iterator.current());
                    } else if (iterator.current().size() > name.size() + 1) {
                        if (iterator.current()[name.size() + 1] == '=') {
                            stream.str(iterator.current().substr(2 + name.size()));
                        } else {
                            stream.str(iterator.current().substr(1 + name.size()));
                        }
                    } else {
                        std::ostringstream err_msg;
                        err_msg << "I don't understand how this happened.";
                        throw Exception(err_msg.str());
                    }

                    a->parse(stream);
                    break;
                }
            }

            if (success) {
                break;
            }
        }

        if (!success) {
            std::ostringstream err_msg;
            err_msg << "unrecognized argument: `" << iterator.current() << "`.";
            throw Exception(err_msg.str());
        }
    }

    void Parser::_parse_positional(ArgIterator& iterator, std::vector<Arg*>::iterator& positional) {
        if (positional == this->positional_args.end()) {
            std::ostringstream err_msg;
            err_msg << "too many positional arguments: `" << iterator.current() << "`.";
            throw Exception(err_msg.str());
        }

        Arg* arg = *positional;
        std::istringstream stream(iterator.current());
        arg->parse(stream);
        positional++;
    }

    void Parser::parse(int argc, char* argv[]) {
        ArgIterator iterator(argc, argv);
        std::vector<Arg*>::iterator positional = this->positional_args.begin();
        this->_parse(iterator, positional);
    }

    void Parser::parse(std::vector<std::string> args) {
        ArgIterator iterator(args);
        std::vector<Arg*>::iterator positional = this->positional_args.begin();
        this->_parse(iterator, positional);
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
}
