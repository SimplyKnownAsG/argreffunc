#pragma once

#include "arf/Arg.hpp"
#include "arf/ArgIterator.hpp"
#include "arf/FuncArg.hpp"
#include "arf/RefArg.hpp"
#include "arf/SwitchArg.hpp"

#include <functional>
#include <vector>

namespace arf {

    /**
     * @brief Main parser class and primary user interface.
     *
     * The Parser class creates and manages `Arg`s, and parses the user-supplied arguments.
     */
    class Parser {
    private:
        std::string const program;

        std::vector<Arg*> args;

        void _parse(ArgIterator& iterator);

    public:
        /**
         * @brief Create a parser with a program name.
         *
         * @param program name of the program, used for help output
         */
        Parser(std::string program);

        /**
         * @brief Add a positional argument with a given name and a reference.
         *
         * `argreffunc` will automatically convert the command line argument to the specified type
         * and assign the value to `refval`.
         *
         * @tparam T type of the argument
         * @param name name of the argument
         * @param help help message
         * @param refval reference (`&`) of the value to assign
         */
        template<typename T>
        void add_positional(std::string name, std::string help, T& refval) {
            auto arg = new RefArg<T>(name, true, true, help, refval);
            this->args.push_back(arg);
        };

        /**
         * @brief Add a positional argument with a given name and function.
         *
         * `argreffunc` will automatically convert the command line argument to the specified type
         * and pass that value into `func`.
         *
         * @tparam T type the argument should be cast to prior to calling the function
         * @param name name of the argument
         * @param help help message
         * @param func user specified function for interpreting the command line argument
         */
        template<typename T>
        void add_positional(std::string name, std::string help, std::function<void(T)> func) {
            auto arg = new FuncArg<T>(name, true, true, help, func);
            this->args.push_back(arg);
        };

        /**
         * @brief Add a named argument with a reference.
         *
         * `argreffunc` will automatically convert the command line argument to the specified type
         * and assign the value to `refval`.
         *
         * @tparam T type of the arugment
         * @param name name of the argument
         * @param help help message
         * @param refval reference (`&`) of the value to assign
         *
         * @return a reference to the Arg that was created. This is done so that you could add
         *         aliases to the argument.
         */
        template<typename T>
        Arg& add(std::string name, std::string help, T& refval) {
            auto arg = new RefArg<T>(name, false, false, help, refval);
            this->args.push_back(arg);
            return *arg;
        };

        /**
         * @brief Add a named argument with a function to interpret the value.
         *
         * `argreffunc` will automatically convert the command line argument to the specified type
         * and pass that value into `func`.
         *
         * @tparam T type the argument should be cast to prior to calling the function
         * @param name name of the argument
         * @param help help message
         * @param func user specified function for interpreting the command line argument
         *
         * @return a reference to the Arg that was created. This is done so that you could add
         *         aliases to the argument.
         */
        template<typename T>
        Arg& add(std::string name, std::string help, std::function<void(T)> func) {
            auto arg = new FuncArg<T>(name, false, false, help, func);
            this->args.push_back(arg);
            return *arg;
        };

        /**
         * @brief Add a named argument with a function to interpret the value.
         *
         * A SwitchArg when found invokes the specified function.
         *
         * @param name name of the argument
         * @param help help message
         * @param func user specified function for interpreting the command line argument
         *
         * @return a reference to the Arg that was created. This is done so that you could add
         *         aliases to the argument.
         */
        Arg& add(std::string name, std::string help, std::function<void()> func) {
            auto arg = new SwitchArg(name, help, func);
            this->args.push_back(arg);
            return *arg;
        };

        /**
         * @brief parse the specified arguments. Assume that it should start at argument 1.
         *
         * This method will iterate over all the arguments until they are consumed. Afterward,
         * reference arguments will be assigned, and function arguments will have been called
         * accordingly.
         *
         * @param argc number of arguments as passed from `main()`
         * @param argv[] argument values as passed from `main()`
         */
        void parse(int argc, char* argv[]);

        /**
         * @brief parse the specified arguments.
         *
         * This method will iterate over all the arguments until they are consumed. Afterward,
         * reference arguments will be assigned, and function arguments will have been called
         * accordingly.
         *
         * @param argc number of arguments as passed from `main()`
         * @param argv[] argument values as passed from `main()`
         */
        void parse(std::vector<std::string> args);

        /**
         * @brief Print help message to std::cout.
         */
        void print_help();

        /**
         * @brief Print help message to the provided stream
         *
         * @param stream std::ostream to write to
         */
        void print_help(std::ostream& stream);
    };
}
