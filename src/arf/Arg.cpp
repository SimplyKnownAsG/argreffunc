#include "arf/Arg.hpp"
#include "arf/Exception.hpp"

namespace arf {

    Arg::Arg(std::string name) {
        this->names.push_back(name);
    }

    void Arg::parse(std::istringstream& stream) {
        this->_parse(stream);
        if (stream.bad()) {
            std::ostringstream err_msg;
            err_msg << "Failed to parse `" << this->names[0] << "`, stream state is bad.";
            throw Exception(err_msg.str());
        }
        if (!stream.eof()) {
            std::ostringstream err_msg;
            err_msg << "Failed to parse `" << this->names[0]
                    << "`, not at end of stream: " << stream.str();
            throw Exception(err_msg.str());
        }
    }

    void Arg::add_alias(std::string alias) {
        if (alias.size() == 1) {
            this->aliases.push_back(alias);
        } else {
            this->names.push_back(alias);
        }
    }
}
