# if either tests or example should be built - then add some extra flags
if(USE_TESTS OR USE_EXAMPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wunused-variable")
endif()

add_subdirectory(options)

if(USE_TESTS)
    add_subdirectory(tests)
endif()

if(USE_EXAMPLE)
    add_subdirectory(example)
endif()
