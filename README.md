# Options

[![Cpp Standard](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)
[![Coverage Status](https://coveralls.io/repos/github/opokatech/options/badge.svg?branch=master)](https://coveralls.io/github/opokatech/options?branch=master)

This is yet another alternative for parsing command line arguments in C++.

The main design criteria were:

* Fast to recompile the main. The library has very small footprint as regards to implicitly included headers. The main using this library includes indirectly only `string` and `cstdint`.
* Easily allows validation of passed values.
* Ease of declaring options and accessing them.
* Requires C++11 or higher.

## How to use it

A [simple example](src/example/example_simple.cpp), without validation looks as follows:

```cpp
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
```

An example "session" on the console may look like this:

```bash
./build_example_debug/example_simple
Usage: ./build_example_debug/example_simple [options] [-- [positional arguments]]
 -c, --config     M Configuration file
     --count        Number of iterations (default: 10)
 -t, --threshold    Some threshold (default: 3.14)
 -h, --help         This help is accessible via short and long option

./build_example_debug/example_simple -c some_file.cfg -- this is a string
Options:
 config    : some_file.cfg
 count     : 10
 threshold : 3.14
Positional parameters:
 [0]: this
 [1]: is
 [2]: a
 [3]: string



```

A similar example, but with validation of data can be found [here](src/example/example_full.cpp).

## How to compile it

This library can be used in a few ways:

Using cmake the easiest way seems to be using
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) mechanism:

```cmake
include(FetchContent)

FetchContent_Declare(
    options
    GIT_REPOSITORY https://github.com/opokatech/options
    GIT_TAG v3.0.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(options)

# ....

add_executable(example my_main.cpp)
target_link_libraries(example PRIVATE options)
```

Another option is to clone the whole repository as a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
and add it via `add_subdirectory`:

```bash
git submodule add https://github.com/opokatech/options externals/options

# optionally checkout some particular version:
cd externals/options
git checkout v3.0.0
```

And then in your CMakeLists.txt:

```cmake
add_subdirectory(external/options) # location of the cloned repository

add_executable(example my_main.cpp)
target_link_libraries(example PRIVATE options)
```

And there is of course the most simple and **not recommended** way:
copying `src/options` directory to your project and adding the files from it to
the compilation process. Either by hand OR by including only the `options` directory
via `add_subdirectory`. After all the whole library consists of just 7 files.

## Some notes

* an option may have an argument or not. An option without an argument is a flag,
* an option is always identified by a long name. A long name is used with 2 dashes in front of it,
  like so `--something`. It may have a single character short version which is then used with a
  single dash in front of it, like so `-s`.

* an option can be mandatory - which means it must be specified,
* it is **not** checked if the defined options are unique,
* it is **not** checked if the default value passes the validation (if used).
