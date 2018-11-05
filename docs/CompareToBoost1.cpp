// clang-format off
#include "arf/Parser.hpp"

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    try {

        arf::Parser parser("Allowed options");

        double compression = 0.0;

        parser.add("help", "produce help message", [&parser]() {
                parser.print_help();
                exit(1);
            });
        parser.add("compression", "set compression level", compression);
        parser.parse(argc, argv);

        cout << "Compression level is " << compression << ".\n";
    }
    catch (exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}
