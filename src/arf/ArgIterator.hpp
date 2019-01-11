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
        size_t _value_start;

        size_t _position;

        ArgType _type;

        std::vector<std::string> arguments;

    public:
        explicit ArgIterator(std::vector<std::string> arguments);

        ArgIterator(int argc, char* argv[]);

        bool next();

        std::string const& current() const;

        std::unordered_set<Name*> matched_positional_args;

        ArgType const& type() const;

        std::string get_name();

        template<typename T>
        T get_value(std::string const& name) {
            T val;
            if (this->current().size() <= this->_value_start) {
                this->next();
            }

            if (this->_type == ArgType::None) {
                throw Exception("Failed to parse `" + name + "`, missing value");
            }

            std::istringstream stream(this->current());
            stream.seekg(this->_value_start, std::ios::cur);
            stream >> val;

            this->_value_start = this->current().size() + 1;

            if (stream.bad()) {
                std::ostringstream err_msg;
                err_msg << "Failed to parse `" << name << "`, stream state is bad.";
                throw Exception(err_msg.str());
            }

            if (!stream.eof()) {
                std::ostringstream err_msg;
                err_msg << "Failed to parse `" << name
                        << "`, not at end of stream, remaining value: `" << stream.str()
                        << "`. Gathered value: `" << val << "`.";
                throw Exception(err_msg.str());
            }

            return val;
        };

        template<typename T>
        void get_values(std::string const& name, std::vector<T>& values) {
            values.push_back(this->get_value<T>(name));
            while (this->next() && this->type() == ArgType::Positional) {
                values.push_back(this->get_value<std::string>("values"));
            }
        };
    };
}
