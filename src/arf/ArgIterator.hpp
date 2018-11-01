#pragma once

#include "arf/ArgType.hpp"
#include "arf/Exception.hpp"
#include "arf/Name.hpp"

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace arf {

    /**
     * @brief Class for internal use which iterates over all the command line arguments supplied by
     * a user.
     */
    class ArgIterator {
    private:
        int _position;

        ArgType _type;

        std::vector<std::string> arguments;

        friend class Name;

        int value_start;

    public:
        explicit ArgIterator(std::vector<std::string> arguments);

        ArgIterator(int argc, char* argv[]);

        bool next();

        std::string const& current() const;

        std::unordered_set<Name*> matched_positional_args;

        ArgType const& type() const;

        std::string get_name();

        template<typename T>
        T get_value(Name& name) {
            T val;
            if (this->current().size() <= this->value_start) {
                this->next();
            }

            std::istringstream stream(this->current());
            stream.seekg(this->value_start, std::ios::cur);
            stream >> val;

            if (stream.bad()) {
                std::ostringstream err_msg;
                err_msg << "Failed to parse `" << name.names[0] << "`, stream state is bad.";
                throw Exception(err_msg.str());
            }

            if (!stream.eof()) {
                std::ostringstream err_msg;
                err_msg << "Failed to parse `" << name.names[0]
                        << "`, not at end of stream, remaining value: `" << stream.str()
                        << "`. Gathered value: `" << val << "`.";
                throw Exception(err_msg.str());
            }

            /* this->next(); */
            return val;
        };
    };
}
