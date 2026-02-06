cmake_minimum_required(VERSION 3.15)
project(ID3 VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(SOURCES
    source/File.cpp
    source/MP3File.cpp
    source/MP3.cpp
    source/MP3Frame.cpp
    source/MP3FrameHeader.cpp
    source/ID3.cpp
    source/ID3v1.cpp
    source/ID3v10.cpp
    source/ID3v11.cpp
    source/ID3v2.cpp
    source/ID3v20.cpp
    source/ID3v23.cpp
    source/ID3v24.cpp
)

# Header files
set(HEADERS
    source/File.hpp
    source/MP3File.hpp
    source/MP3.hpp
    source/MP3Frame.hpp
    source/MP3FrameHeader.hpp
    source/ID3.hpp
    source/ID3v1.hpp
    source/ID3v10.hpp
    source/ID3v11.hpp
    source/ID3v2.hpp
    source/ID3v20.hpp
    source/ID3v23.hpp
    source/ID3v24.hpp
    source/ID3Export.h
)

# Include directory (headers without extension)
set(PUBLIC_HEADERS
    include/File
    include/MP3File
    include/MP3
    include/MP3Frame
    include/MP3FrameHeader
    include/ID3
    include/ID3v1
    include/ID3v10
    include/ID3v11
    include/ID3v2
    include/ID3v20
    include/ID3v23
    include/ID3v24
)

# Static library
add_library(ID3_static STATIC ${SOURCES} ${HEADERS})
target_include_directories(ID3_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(ID3_static PROPERTIES
    OUTPUT_NAME ID3
    POSITION_INDEPENDENT_CODE ON
)

# Shared library
add_library(ID3_shared SHARED ${SOURCES} ${HEADERS})
target_include_directories(ID3_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_definitions(ID3_shared PRIVATE ID3_BUILD_SHARED)
set_target_properties(ID3_shared PROPERTIES
    OUTPUT_NAME ID3
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# Platform-specific settings
if(UNIX AND NOT APPLE)
    set_target_properties(ID3_shared PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
elseif(APPLE)
    set_target_properties(ID3_shared PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()

# Alias targets
add_library(ID3::static ALIAS ID3_static)
add_library(ID3::shared ALIAS ID3_shared)

# Installation
install(TARGETS ID3_static ID3_shared
    EXPORT ID3Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${PUBLIC_HEADERS}
    DESTINATION include/ID3
)

install(EXPORT ID3Targets
    FILE ID3Targets.cmake
    NAMESPACE ID3::
    DESTINATION lib/cmake/ID3
)

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ID3ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ID3Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ID3Config.cmake"
    INSTALL_DESTINATION lib/cmake/ID3
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ID3Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ID3ConfigVersion.cmake"
    DESTINATION lib/cmake/ID3
)
