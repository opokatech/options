set(COMMON_COMPILE_FLAGS -Wall -Wextra -Wpedantic -Wshadow -Wno-sign-compare -Wno-unused-parameter)

add_library(options_compile_flags INTERFACE)
target_compile_options(options_compile_flags INTERFACE ${COMMON_COMPILE_FLAGS})

add_library(options_tests_compile_flags INTERFACE)
target_compile_options(options_tests_compile_flags INTERFACE ${COMMON_COMPILE_FLAGS})
