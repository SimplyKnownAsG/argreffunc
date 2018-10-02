#pragma once

#include <exception>
#include <string>

namespace arf {
    class Exception : public std::exception {
    protected:
        std::string _message;

    public:
        explicit Exception(const std::string& message);

        explicit Exception(const char* message);

        const char* what() const noexcept override;
    };
}
