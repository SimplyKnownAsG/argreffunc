// clang-format off
#include "arf/Parser.hpp"
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    // Wrap everything in a try block.  Do this every time,
    // because exceptions will be thrown for problems.
    try {

        // Define the command line Parser.
        arf::Parser parser("Command description message");

        // Define a value argument and add it to the command line.
        string name = "homer";
        parser.add("name", "Name to print", name)
              .add_alias("n");

        // Define a switch and add it to the command line.
        bool reverseName = false;
        parser.add("reverse", "Print name backwards",
                   [&reverseName]() { reverseName = true; })
              .add_alias("r");

        // Parse the args.
        parser.parse(argc, argv);

        // Do what you intend too...
        if (reverseName)
        {
            reverse(name.begin(), name.end());
            cout << "My name (spelled backwards) is: " << name << endl;
        }
        else
            cout << "My name is: " << name << endl;

    } catch (arf::Exception& e) // catch any exceptions
    { cerr << "error: " << e.what() << endl; }
}
