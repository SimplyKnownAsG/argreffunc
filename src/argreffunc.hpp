
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

    class Arg {
    public:
        Arg(std::string name)
          : name(name){};
        const std::string name;
        virtual void parse(std::istream& stream) = 0;
    };

    template<typename T>
    class RefArg : public Arg {
    public:
        T& val;
        RefArg(const std::string name, T& arg)
          : Arg(name)
          , val(arg){};

        void parse(std::istream& stream) override {
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
        void parse(std::istream& stream) override {
            T value;
            stream >> value;
            this->func(value);
        };
    };

    class Parser {
    private:
        std::vector<Arg*> args;

    public:
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
            for (int ii = 1; ii < argc; ii++) {
                auto arg = this->args.at(ii - 1);
                std::istringstream stream(argv[ii]);
                arg->parse(stream);
                if (stream.bad()) {
                    std::ostringstream msg;
                    msg << "Failed to parse `" << arg->name << "`, stream state is bad.";
                    throw ArfException(msg.str());
                }
                if (!stream.eof()) {
                    std::ostringstream msg;
                    msg << "Failed to parse `" << arg->name << "`, not at end of stream.";
                    throw ArfException(msg.str());
                }
            }
        };
    };
}
