#include <iostream>

#include "options/Converters.hpp"
#include "options/Parser.hpp"

int main(int argc, char *argv[])
{
    Options::Parser args_parser;

    args_parser.add_flag("help", 'h', "This help is accessible via short and long option");
    args_parser.add_flag("verbose", 'v', "Verbose - accessible via -v and --verbose");
    args_parser.add_optional(
        "level", "Debug level, one of none, debug, error - it is checked by the validator", "none",
        [](const std::string &value) { return (value == "none" || value == "debug" || value == "error"); });

    args_parser.add_mandatory("config", 'c', "Configuration file");
    args_parser.add_optional("int", 'i', "Some small integer in range <-10..10> as checked by validator", "4",
                             [](const std::string &value) {
                                 int32_t num = Options::as_int(value);
                                 return num >= -10 && num <= 10; // NOLINT
                             });
    args_parser.add_optional("double", 'd', "Double value > 3.0", "3.14", [](const std::string &value) {
        double num = Options::as_double(value);
        return num > 3.0; // NOLINT
    });

    args_parser.add_optional("bf", "Boolean value", "false");
    args_parser.add_optional("bt", "Boolean value", "true");

    using std::cout;
    using std::endl;

    if (!args_parser.parse(argc, argv) || args_parser.as_bool("help"))
    {
        cout << "Usage: " << argv[0] << " [options] [-- [positional arguments]]" << endl;
        cout << args_parser.get_possible_options() << endl;
        return -1;
    }

    cout << std::boolalpha;
    cout << "Options:" << endl;
    cout << " verbose : " << args_parser.as_bool("verbose") << endl;
    cout << " level   : " << args_parser.as_string("level") << endl;
    cout << " config  : " << args_parser.as_string("config") << endl;
    cout << " int     : " << args_parser.as_int("int") << endl;
    cout << " double  : " << args_parser.as_double("double") << endl;
    cout << " bf      : " << args_parser.as_bool("bf") << endl;
    cout << " bt      : " << args_parser.as_bool("bt") << endl;

    // positional arguments are all the strings after "--" separator, for example:
    // ./example -c config_file.txt -v -- these strings are positional arguments
    if (args_parser.positional_count() > 0)
    {
        cout << "Positional parameters:" << endl;
        for (size_t i = 0; i < args_parser.positional_count(); ++i)
            cout << " [" << i << "]: " << args_parser.positional(i) << endl;
    }
    else
        cout << "No positional parameters." << endl;

    return 0;
}
