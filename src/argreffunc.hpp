
#include <functional>
#include <istream>
#include <limits>
#include <sstream>
#include <vector>

namespace arf {

    class ArfException : public std::exception {
    protected:
        std::string _message;

    public:
        ArfException(const std::string& message)
          : _message(message){};

        ArfException(const char* message)
          : _message(message){};

        const char* what() const noexcept override {
            return this->_message.c_str();
        };
    };

    class ArgIterator {
    private:
        int _position;

        std::vector<std::string> arguments;

    public:
        ArgIterator(std::vector<std::string> arguments)
          : arguments(arguments) {
            this->_position = -1;
        };

        ArgIterator(int argc, char* argv[])
          : arguments(argv + 1, argv + argc) {
            this->_position = -1;
        };

        bool next() {
            return ++_position < this->arguments.size();
        };

        const std::string& current() const {
            return this->arguments.at(this->_position);
        };
    };

    class Arg {
    public:
        const std::string name;

        Arg(std::string name)
          : name(name){};

        void parse(std::istringstream& stream) {
            this->_parse(stream);
            if (stream.bad()) {
                std::ostringstream err_msg;
                err_msg << "Failed to parse `" << this->name << "`, stream state is bad.";
                throw ArfException(err_msg.str());
            }
            if (!stream.eof()) {
                std::ostringstream err_msg;
                err_msg << "Failed to parse `" << this->name
                        << "`, not at end of stream: " << stream.str();
                throw ArfException(err_msg.str());
            }
        };

    protected:
        virtual void _parse(std::istream& stream) = 0;
    };

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

    template<typename T>
    class FuncArg : public Arg {
    public:
        std::function<void(T)> func;
        FuncArg(const std::string name, std::function<void(T)> func)
          : Arg(name)
          , func(func){};
        void _parse(std::istream& stream) override {
            T value;
            stream >> value;
            this->func(value);
        };
    };

    class Parser {
    private:
        std::vector<Arg*> args;

        std::vector<Arg*> positional_args;

        void _parse(ArgIterator& iterator, std::vector<Arg*>::iterator& positional) {
            while (iterator.next()) {
                if (iterator.current()[0] == '-') {
                    if (iterator.current()[1] == '-') {
                        this->_parse_long(iterator);
                    } else {
                        this->_parse_short(iterator);
                    }
                } else {
                    this->_parse_positional(iterator, positional);
                }
            }
            if (positional != this->positional_args.end()) {
                std::ostringstream err_msg;
                err_msg << "too few positional arguments.";
                throw ArfException(err_msg.str());
            }
        };

        void _parse_long(ArgIterator& iterator) {
            bool success = false;
            for (auto a : this->args) {
                /* 2 after leading "--" */
                if (iterator.current().find(a->name) == 2) {
                    success = true;
                    std::istringstream stream;
                    if (iterator.current().size() == a->name.size() + 2) {
                        iterator.next();
                        stream = std::istringstream(iterator.current());
                    } else if (iterator.current().size() > a->name.size() + 2) {
                        if (iterator.current()[a->name.size() + 2] != '=') {
                            std::ostringstream err_msg;
                            err_msg << "expected `=` sign after argument `" << a->name
                                    << "`, but got:" << iterator.current() << "`.";
                            throw ArfException(err_msg.str());
                        }
                        stream = std::istringstream(iterator.current().substr(3 + a->name.size()));
                    } else {
                        std::ostringstream err_msg;
                        err_msg << "I don't understand how this happened.";
                        throw ArfException(err_msg.str());
                    }
                    a->parse(stream);
                    break;
                }
            }
            if (!success) {
                std::ostringstream err_msg;
                err_msg << "unrecognized argument: `" << iterator.current() << "`.";
                throw ArfException(err_msg.str());
            }
        };

        void _parse_short(ArgIterator& iterator){};

        void _parse_positional(ArgIterator& iterator, std::vector<Arg*>::iterator& positional) {
            if (positional == this->positional_args.end()) {
                std::ostringstream err_msg;
                err_msg << "too many positional arguments: `" << iterator.current() << "`.";
                throw ArfException(err_msg.str());
            }

            Arg* arg = *positional;
            std::istringstream stream(iterator.current());
            arg->parse(stream);
            positional++;
        };

    public:
        template<typename T>
        Parser& add_positional(std::string name, T& refval) {
            auto arg = new RefArg<T>(name, refval);
            this->positional_args.push_back(arg);
            return *this;
        };

        template<typename T>
        Parser& add_positional(std::string name, std::function<void(T)> refval) {
            auto arg = new FuncArg<T>(name, refval);
            this->positional_args.push_back(arg);
            return *this;
        };

        template<typename T>
        Parser& add(std::string name, T& refval) {
            auto arg = new RefArg<T>(name, refval);
            this->args.push_back(arg);
            return *this;
        };

        template<typename T>
        Parser& add(std::string name, std::function<void(T)> func) {
            auto arg = new FuncArg<T>(name, func);
            this->args.push_back(arg);
            return *this;
        };

        void parse(int argc, char* argv[]) {
            ArgIterator iterator(argc, argv);
            std::vector<Arg*>::iterator positional = this->positional_args.begin();
            this->_parse(iterator, positional);
        };

        void parse(std::vector<std::string> args) {
            ArgIterator iterator(args);
            std::vector<Arg*>::iterator positional = this->positional_args.begin();
            this->_parse(iterator, positional);
        };
    };
}
