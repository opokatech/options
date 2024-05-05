#include <iostream>

#include "options/Parser.hpp"

int main(int argc, char *argv[])
{
    Options::Parser args_parser;

    args_parser.add_mandatory("config", 'c', "Configuration file");       // --config, -c
    args_parser.add_optional("count", "Number of iterations", "10");      // --count
    args_parser.add_optional("threshold", 't', "Some threshold", "3.14"); // --threshold, -t
    args_parser.add_flag("help", 'h', "This help is accessible via short and long option");

    using std::cout;
    using std::endl;

    if (!args_parser.parse(argc, argv) || args_parser.as_bool("help"))
    {
        cout << "Usage: " << argv[0] << " [options] [-- [positional arguments]]" << endl;
        cout << args_parser.get_possible_options() << endl;
        return -1;
    }

    // Print all options
    cout << std::boolalpha;
    cout << "Options:" << endl;
    cout << " config    : " << args_parser.as_string("config") << endl;
    cout << " count     : " << args_parser.as_int("count") << endl;
    cout << " threshold : " << args_parser.as_double("threshold") << endl;

    // positional arguments are all the strings after "--" separator, for example:
    // ./example_simple -c config_file.txt -- these strings are positional arguments
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
