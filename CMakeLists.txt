cmake_minimum_required(VERSION 3.15)
project(ID3 VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/library)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/library)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binary)

# Options
option(BUILD_APPLICATIONS "Build ID3 applications" ON)
option(BUILD_TESTS "Build ID3 tests" OFF)

# Build library subproject
add_subdirectory(library)

# Build applications subproject (depends on library)
if(BUILD_APPLICATIONS)
    # Export targets for applications to find
    export(TARGETS ID3_static ID3_shared
        NAMESPACE ID3::
        FILE "${CMAKE_BINARY_DIR}/ID3Targets.cmake"
    )
    
    # Configure package for applications
    set(ID3_DIR "${CMAKE_BINARY_DIR}")
    add_subdirectory(application)
endif()

# Build tests subproject
if(BUILD_TESTS)
    # Export targets for tests to find
    export(TARGETS ID3_static ID3_shared
        NAMESPACE ID3::
        FILE "${CMAKE_BINARY_DIR}/ID3Targets.cmake"
    )
    
    set(ID3_DIR "${CMAKE_BINARY_DIR}")
    add_subdirectory(test)
endif()

