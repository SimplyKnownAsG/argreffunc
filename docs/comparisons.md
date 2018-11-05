# Comparisons to existing software
There are already lots of parsers out there. So, what's so great about `argreffunc`?

It is mainly just different. The sections below take examples from other projects
and convert them to use `argreffunc`.

## [boost::program_options](https://www.boost.org/doc/libs/1_68_0/doc/html/program_options/tutorial.html)
Might as well start with boost.

<table>
<tr><th>`boost::program_options`</th><th>`argreffunc`</th></tr>
<tr><td>
~~~~{.cpp}
// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* The simplest usage of the library.
 */

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
using namespace std;

int main(int ac, char* av[])
{
    try {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("compression", po::value<double>(), "set compression level")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("compression")) {
            cout << "Compression level was set to " 
                 << vm["compression"].as<double>() << ".\n";
        } else {
            cout << "Compression level was not set.\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}
~~~~
</td><td>
There are two ways to accmplish this. The first uses a `std::function<void()>` to assign the `help`
boolean to true.

\include CompareToBoost0.cpp

</td></tr>
<tr><td/><td>
The second is to uses a `std::function<void()>` that prints help and exits.

\include CompareToBoost1.cpp
</td></tr>
</table>

Note that in `argreffunc`, the programmer does not necessarily know if the user supplied the
argument or not. This is the intended behavior. You can set a default value to the argument prior to
parsing the arguments.

There is also a lot more that can be done with `boost::program_options`. In fact, it looks like
there is a mechanism to use pointers with `po::value<T>(T*)`.

## TCLAP

<table>
<tr><th>`TCLAP`</th><th>`argreffunc`</th></tr>
<tr><td>
~~~~{.cpp}
#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>

using namespace TCLAP;
using namespace std;

int main(int argc, char** argv)
{
    // Wrap everything in a try block.  Do this every time, 
    // because exceptions will be thrown for problems. 
    try {  

    // Define the command line object.
    CmdLine cmd("Command description message", ' ', "0.9");

    // Define a value argument and add it to the command line.
    ValueArg<string> nameArg("n","name","Name to print",true,"homer","string");
    cmd.add( nameArg );

    // Define a switch and add it to the command line.
    SwitchArg reverseSwitch("r","reverse","Print name backwards", false);
    cmd.add( reverseSwitch );

    // Parse the args.
    cmd.parse( argc, argv );

    // Get the value parsed by each arg. 
    string name = nameArg.getValue();
    bool reverseName = reverseSwitch.getValue();

    // Do what you intend too...
    if ( reverseName )
    {
        reverse(name.begin(),name.end());
        cout << "My name (spelled backwards) is: " << name << endl;
    }
    else
        cout << "My name is: " << name << endl;


    } catch (ArgException &e)  // catch any exceptions
    { cerr << "error: " << e.error() << " for arg " << e.argId() << endl; }
}
~~~~
</td><td>
\include CompareToTCLAP.cpp
</td></tr>
</table>
