add_library(options_compile_flags INTERFACE)
target_compile_options(options_compile_flags INTERFACE -Wall -Wextra -Wpedantic)

add_library(options_tests_compile_flags INTERFACE)
target_compile_options(options_tests_compile_flags INTERFACE -Wall -Wextra -Wpedantic)
