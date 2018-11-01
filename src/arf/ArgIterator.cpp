#include "arf/ArgIterator.hpp"

namespace arf {

    ArgIterator::ArgIterator(std::vector<std::string> arguments)
      : arguments(arguments) {
        this->_position = -1;
    }

    ArgIterator::ArgIterator(int argc, char* argv[])
      : arguments(argv + 1, argv + argc) {
        this->_position = -1;
    }

    bool ArgIterator::next() {
        this->value_start = 0;
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
            begin = 1;
            len = 1;
            break;
        default:
            throw Exception("Cannot get name of positional argument");
        }

        this->value_start = begin + len;
        if (this->value_start < this->current().size() &&
            this->current()[this->value_start] == '=') {
            this->value_start += 1;
        }
        return this->current().substr(begin, len);
    }
}
