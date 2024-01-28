include(FetchContent)

set(CATCH2_GIT_TAG v3.5.2)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG ${CATCH2_GIT_TAG}
    GIT_SHALLOW TRUE)

message(STATUS "Making Catch2 ${CATCH2_GIT_TAG} available...")
FetchContent_MakeAvailable(Catch2)
message(STATUS "Catch2 ready")

enable_testing()

add_executable(${PROJECT_NAME}_tests Option_Test.cpp Options_Test.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE options options_tests_compile_flags Catch2WithMain)
