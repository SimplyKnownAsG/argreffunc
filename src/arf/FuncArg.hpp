#pragma once

#include "arf/Arg.hpp"

#include <functional>

namespace arf {
    template<typename T>
    class FuncArg : public Arg {
    private:
        std::function<void(T)> func;

    public:
        FuncArg(const std::string name, std::function<void(T)> func)
          : Arg(name)
          , func(func){};

        void _parse(std::istream& stream) override {
            T value;
            stream >> value;
            this->func(value);
        };
    };
}
