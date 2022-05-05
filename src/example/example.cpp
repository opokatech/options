#include <iostream>

#include "options/Converters.hpp"
#include "options/Options.hpp"

int main(int argc, char *argv[])
{
    using std::cout;
    using std::endl;

    Options::Options o;

    o.add_flag("help", 'h', "This help is accessible via short and long option");
    o.add_flag("verbose", 'v', "Verbose - accessible via -v and --verbose");
    o.add_optional(
        "dlevel", "Debug level, one of none, debug, error - it is checked by the validator", "none",
        [](const std::string &v) { return (v == "none" || v == "debug" || v == "error"); });

    o.add_mandatory("config", 'c', "Configuration file");
    o.add_optional("int", 'i', "Some small integer in range <-10..10> as checked by validator", "4",
                   [](const std::string &s) {
                       int32_t i = Options::as_int(s);
                       return i >= -10 && i <= 10;
                   });
    o.add_optional("double", 'd', "Double value > 3.0", "3.14", [](const std::string &s) {
        double d = Options::as_double(s);
        return d > 3.0;
    });

    o.add_optional("bf", "Boolean value", "false");
    o.add_optional("bt", "Boolean value", "true");

    if (!o.parse(argc, argv) || o.as_bool("help"))
    {
        cout << "Usage: " << argv[0] << " [options] [-- [positional arguments]]" << endl;
        cout << o.get_possible_options() << endl;
        return -1;
    }

    cout << std::boolalpha;
    cout << "Options:" << endl;
    cout << " verbose : " << o.as_bool("verbose") << endl;
    cout << " dlevel  : " << o.as_string("dlevel") << endl;
    cout << " config  : " << o.as_string("config") << endl;
    cout << " int     : " << o.as_int("int") << endl;
    cout << " double  : " << o.as_double("double") << endl;
    cout << " bf      : " << o.as_bool("bf") << endl;
    cout << " bt      : " << o.as_bool("bt") << endl;

    // positional arguments are all the strings after "--" separator, for example:
    // ./example -c config_file.txt -v -- these strings are positional arguments
    if (o.positional_count() > 0)
    {
        cout << "Positional parameters:" << endl;
        for (size_t i = 0; i < o.positional_count(); ++i)
            cout << " [" << i << "]: " << o.positional(i) << endl;
    }
    else
        cout << "No positional parameters." << endl;

    return 0;
}
