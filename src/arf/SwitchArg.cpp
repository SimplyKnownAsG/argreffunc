#include "arf/SwitchArg.hpp"

namespace arf {
    SwitchArg::SwitchArg(const std::string name, std::string help, std::function<void()> func)
      : Arg(name, false, false, help)
      , func(func){};

    void SwitchArg::parse_hook(ArgIterator& iterator) {
        this->func();
    }
}
