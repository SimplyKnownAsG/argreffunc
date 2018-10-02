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
        return ++_position < this->arguments.size();
    }

    const std::string& ArgIterator::current() const {
        return this->arguments.at(this->_position);
    }
}
