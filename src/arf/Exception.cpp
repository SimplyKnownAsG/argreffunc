#include "arf/Exception.hpp"

namespace arf {
    Exception::Exception(const std::string& message)
      : _message(message) {}

    Exception::Exception(const char* message)
      : _message(message) {}

    const char* Exception::what() const noexcept {
        return this->_message.c_str();
    }
}
