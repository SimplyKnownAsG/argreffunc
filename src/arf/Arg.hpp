#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace arf {
    class Arg {
    public:
        std::vector<std::string> names;

        std::vector<std::string> aliases;

        explicit Arg(std::string name);

        void parse(std::istringstream& stream);

        void add_alias(std::string alias);

        template<typename... Alias>
        void add_alias(std::string alias, Alias... aliases) {
            this->add_alias(alias);
            this->add_alias(aliases...);
        };

    protected:
        virtual void _parse(std::istream& stream) = 0;
    };
}
