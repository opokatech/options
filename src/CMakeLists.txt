# if either tests or example should be built - then add some extra flags
add_subdirectory(options)

if(USE_TESTS)
    add_subdirectory(tests)
endif()

if(USE_EXAMPLE)
    add_subdirectory(example)
endif()
