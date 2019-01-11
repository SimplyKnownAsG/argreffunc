#include "arf/FuncArg.hpp"

namespace arf {
    template<>
    void FuncArg<ArgIterator&>::parse_hook(ArgIterator& iterator) {
        this->func(iterator);
    };
}
