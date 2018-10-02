#pragma once

#include <string>
#include <vector>

namespace arf {

    class ArgIterator {
    private:
        int _position;

        std::vector<std::string> arguments;

    public:
        explicit ArgIterator(std::vector<std::string> arguments);

        ArgIterator(int argc, char* argv[]);

        bool next();

        const std::string& current() const;
    };
}
