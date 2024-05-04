add_library(options STATIC Converters.cpp Option.cpp Parser.cpp)
target_include_directories(options PUBLIC ..)
target_link_libraries(options PRIVATE options_compile_flags)
