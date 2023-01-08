include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.2.1
    GIT_SHALLOW TRUE)

message(STATUS "Making Catch2 available...")
FetchContent_MakeAvailable(Catch2)
message(STATUS "Catch2 ready")

enable_testing()

add_executable(${PROJECT_NAME}_tests Option_Test.cpp Options_Test.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE options Catch2WithMain)
