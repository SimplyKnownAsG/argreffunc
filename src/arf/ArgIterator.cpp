#include "arf/ArgIterator.hpp"

namespace arf {

    ArgIterator::ArgIterator(std::vector<std::string> arguments)
      : _value_start(0)
      , _position(-1)
      , _type(ArgType::None)
      , arguments(arguments) {}

    ArgIterator::ArgIterator(int argc, char* argv[])
      : _value_start(0)
      , _position(-1)
      , _type(ArgType::None)
      , arguments(argv + 1, argv + argc) {}

    bool ArgIterator::next() {
        if (this->_value_start != 0 && this->_value_start < this->current().size()) {
            // this is checking for short alias usage. it does not progress if a short alias has
            // been used and the end of the argument hasn't been reached. For example
            //     -vvvf=filename
            //      ^               parse the v
            //       ^^^            continue parsing current string
            //                   ^  move on to next argument
            return true;
        }
        this->_value_start = 0;
        bool has_next = ++_position < this->arguments.size();

        if (!has_next) {
            this->_type = ArgType::None;
            return false;
        }

        if (this->current().at(0) == '-') {
            if (this->current().at(1) == '-') {
                this->_type = ArgType::Long;
            } else {
                this->_type = ArgType::Short;
            }
        } else {
            this->_type = ArgType::Positional;
        }

        return true;
    }

    std::string const& ArgIterator::current() const {
        return this->arguments.at(this->_position);
    }

    ArgType const& ArgIterator::type() const {
        return this->_type;
    }

    std::string ArgIterator::get_name() {
        std::string name;
        size_t begin, len;

        switch (this->_type) {
        case ArgType::Long:
            begin = 2;
            len = this->current().find("=") - 2;
            break;
        case ArgType::Short:
            begin = this->_value_start; // allows us to march
            if (begin == 0) {
                begin += 1;
            }
            len = 1;
            break;
        default:
            throw Exception("Cannot get name of positional argument");
        }

        this->_value_start = begin + len;
        if (this->_value_start < this->current().size() &&
            this->current()[this->_value_start] == '=') {
            this->_value_start += 1;
        }
        return this->current().substr(begin, len);
    }
}
