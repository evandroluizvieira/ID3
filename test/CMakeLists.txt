# ID3 Tests Subproject
cmake_minimum_required(VERSION 3.15)
project(ID3Tests VERSION 1.0.0 LANGUAGES CXX)

# When built as standalone project, find the library
# When built as subproject, ID3::static is already available
if(NOT TARGET ID3::static)
    find_package(ID3 REQUIRED)
endif()

# Find GTest
find_package(GTest)

if(NOT GTest_FOUND)
    message(STATUS "GTest not found, downloading from GitHub...")
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(googletest)
endif()

# Enable testing
enable_testing()

# Test executable with empty test (to verify infrastructure)
add_executable(id3_tests main_test.cpp)
target_link_libraries(id3_tests 
    PRIVATE 
        ID3::static
        GTest::gtest_main
)
target_include_directories(id3_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(id3_tests)

message(STATUS "Test infrastructure configured with empty test")
