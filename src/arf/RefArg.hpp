#pragma once

#include "arf/Arg.hpp"

namespace arf {
    template<typename T>
    class RefArg : public Arg {
    private:
        T& val;

    public:
        RefArg(const std::string name, T& arg)
          : Arg(name)
          , val(arg){};

        void _parse(std::istream& stream) override {
            stream >> this->val;
        }
    };
}
