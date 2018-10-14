#pragma once

#include <string>
#include <vector>

namespace arf {

    /**
     * @brief Class for internal use which iterates over all the command line arguments supplied by
     * a user.
     */
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
