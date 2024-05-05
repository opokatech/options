# Options

[![Cpp Standard](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)
[![Coverage Status](https://coveralls.io/repos/github/opokatech/options/badge.svg?branch=master)](https://coveralls.io/github/opokatech/options?branch=master)

This is yet another alternative for parsing command line options in C++.

The main design criteria were:

* fast to recompile,
* ease of declaring options and accessing them,
* only C++11 or higher

An example with comments can be found in [src/example/example.cpp](src/example/example.cpp).

Some notes:

* an option may have an argument or not. An option without an argument is a flag,
* an option is always identified by a long name. A long name is used with 2 dashes in front of it,
  like so `--something`. It may have a single character short version which is then used with a
  single dash in front of it, like so `-s`.

* an option can be mandatory - which means it must be specified,
* it is **not** checked if the defined options are unique,
* it is **not** checked if the default value passes the validation (if used).

## Usage

This library can be used in a few ways:

A simplest way is to copy `src/options` directory to your project and add the files in there to the compilation process either by hand OR by including only this directory via `add_subdirectory`:

Another option is to clone the whole repository and add it via `add_subdirectory`:

```cmake
add_subdirectory(external/options) # location of cloned repository

add_executable(example example.cpp)
target_link_libraries(example PRIVATE options)
```
