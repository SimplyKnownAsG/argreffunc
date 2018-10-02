::

                               __  __                  
                              / _|/ _|                 
      __ _ _ __ __ _ _ __ ___| |_| |_ _   _ _ __   ___ 
     / _` | '__/ _` | '__/ _ \  _|  _| | | | '_ \ / __|
    | (_| | | | (_| | | |  __/ | | | | |_| | | | | (__ 
     \__,_|_|  \__, |_|  \___|_| |_|  \__,_|_| |_|\___|
                __/ |                                  
               |___/                                   


``argreffunc`` is a c++ library for parsing command line arguments.

.. image:: https://travis-ci.org/SimplyKnownAsG/argreffunc.svg?branch=master
    :target: https://travis-ci.org/SimplyKnownAsG/argreffunc


.. image:: https://ci.appveyor.com/api/projects/status/github/SimplyKnownAsG/argreffunc?branch=master&svg=true
    :target: https://ci.appveyor.com/project/SimplyKnownAsG/argreffunc


.. contents:: Table of Contents
    :backlinks: top


Quickstart
==========

::

    #include "arf/argreffunc.hpp"

    int main(int argc, char* argv[]) {
        arf::Parser parser;

        std::string name = "you there"; // default value
        parser.add_argument("name", name)
              .add_alias("n");

        std::string domain;
        int port = 22;
        parser.add<std::string>("url", [&](std::string url) -> void {
            auto colon_loc = url.find(':');
            domain = url.substr(0, colon_loc);
            if (colon_loc != std::string::npos) {
                auto substring = url.substr(colon_loc + 1, url.size() - colon_loc - 1);
                port = std::stoi(substring);
            }
        });

        std::cout << name << "@" << domain << ":" << port << "\n"
    }

Getting the code
================

CMake
-----
1. Use ``execute_process`` and ``add_subdirectory``. This seems to be the easiest, but if there is a
   better way, let us know how and submit a patch.::

    # Recommend putting this into a `<root>/3rd-party/CMakeLists.txt`

    if(NOT EXISTS ${CMAKE_BINARY_DIR}/3rd-party/argreffunc)
            execute_process(
                    COMMAND git clone https://github.com/SimplyKnownAsG/argreffunc.git
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    add_subdirectory(${CMAKE_BINARY_DIR}/3rd-party/argreffunc/src
                     ${CMAKE_BINARY_DIR}/3rd-party/argreffunc/build)


2. Use ``file(DOWNLOAD)``.::

    file(DOWNLOAD
         https://raw.githubusercontent.com/SimplyKnownAsG/argreffunc/master/src/arf/argreffunc.hpp
         ${CMAKE_BINARY_DIR}/3rd-party/arf/argreffunc.hpp)


3. Use ``ExternalProject_Add```.

Otherwise
---------
1. Download with ``wget``/``curl``
2. ``git clone``/``cmake -Bbuild -H.``/``cmake --build build --target install``

