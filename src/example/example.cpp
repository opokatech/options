#include <iostream>

#include "options/Options.hpp"

int main(int argc, char *argv[])
{
    using std::cout;
    using std::endl;

    Options::Options o;

    o.add({"help", 'h', "This help is accessible via short and long option"});
    o.add({"verbose", 'v', "Verbose - accessible via -v and --verbose"});
    o.add({"debug_level", "Debug level, one of none,debug,error - it is checked by validator"})
        .argument()
        .default_value("none")
        .validator(
            [](const std::string &v) { return (v == "none" || v == "debug" || v == "error"); });
    o.add({"config", 'c', "Configuration file"}).mandatory().argument();
    o.add({"int", 'i', "Some small integer in range <-10..10> as checked by validator"})
        .default_value("4")
        .argument()
        .validator([](const std::string &s) {
            int32_t i = Options::Option::as_int(s);
            return i >= -10 && i <= 10;
        });
    o.add({"double", 'd', "Double value > 3.0"})
        .default_value("3.14")
        .argument()
        .validator([](const std::string &s) {
            double d = Options::Option::as_double(s);
            return d > 3.0;
        });

    o.add({"bool", "Boolean value"}).argument();

    if (!o.parse(argc, argv) || o.as_bool("help"))
    {
        cout << "Usage: " << argv[0] << " ... options" << endl;
        cout << o.possible_options() << endl;
        return -1;
    }

    cout << std::boolalpha;
    cout << "verbose    : " << o.as_bool("verbose") << endl;
    cout << "debug_level: " << o.as_string("debug_level") << endl;
    cout << "config     : " << o.as_string("config") << endl;
    cout << "int        : " << o.as_int("int") << endl;
    cout << "double     : " << o.as_double("double") << endl;
    cout << "bool       : " << o.as_bool("bool") << endl;

    return 0;
}
