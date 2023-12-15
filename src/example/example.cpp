#include <iostream>

#include "options/Converters.hpp"
#include "options/Options.hpp"

int main(int argc, char *argv[])
{
    using std::cout;
    using std::endl;

    Options::Options opts;

    opts.add_flag("help", 'h', "This help is accessible via short and long option");
    opts.add_flag("verbose", 'v', "Verbose - accessible via -v and --verbose");
    opts.add_optional(
        "level", "Debug level, one of none, debug, error - it is checked by the validator", "none",
        [](const std::string &value) { return (value == "none" || value == "debug" || value == "error"); });

    opts.add_mandatory("config", 'c', "Configuration file");
    opts.add_optional("int", 'i', "Some small integer in range <-10..10> as checked by validator", "4",
                      [](const std::string &value) {
                          int32_t num = Options::as_int(value);
                          return num >= -10 && num <= 10; // NOLINT
                      });
    opts.add_optional("double", 'd', "Double value > 3.0", "3.14", [](const std::string &value) {
        double num = Options::as_double(value);
        return num > 3.0; // NOLINT
    });

    opts.add_optional("bf", "Boolean value", "false");
    opts.add_optional("bt", "Boolean value", "true");

    if (!opts.parse(argc, argv) || opts.as_bool("help"))
    {
        cout << "Usage: " << argv[0] << " [options] [-- [positional arguments]]" << endl;
        cout << opts.get_possible_options() << endl;
        return -1;
    }

    cout << std::boolalpha;
    cout << "Options:" << endl;
    cout << " verbose : " << opts.as_bool("verbose") << endl;
    cout << " level   : " << opts.as_string("level") << endl;
    cout << " config  : " << opts.as_string("config") << endl;
    cout << " int     : " << opts.as_int("int") << endl;
    cout << " double  : " << opts.as_double("double") << endl;
    cout << " bf      : " << opts.as_bool("bf") << endl;
    cout << " bt      : " << opts.as_bool("bt") << endl;

    // positional arguments are all the strings after "--" separator, for example:
    // ./example -c config_file.txt -v -- these strings are positional arguments
    if (opts.positional_count() > 0)
    {
        cout << "Positional parameters:" << endl;
        for (size_t i = 0; i < opts.positional_count(); ++i)
            cout << " [" << i << "]: " << opts.positional(i) << endl;
    }
    else
        cout << "No positional parameters." << endl;

    return 0;
}
