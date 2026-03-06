# ID3 Applications Subproject
cmake_minimum_required(VERSION 3.15)
project(ID3Applications VERSION 1.0.0 LANGUAGES CXX)

# Find ID3 library
find_package(ID3 REQUIRED)

# Application executables
set(APPLICATIONS
    id3_scan_id3
    id3_remove_id3v1
    id3_remove_id3v2
    id3_full_process
    id3_convert_v10_to_v20
    id3_convert_v10_to_v23
    id3_convert_v10_to_v24
    id3_convert_v11_to_v20
    id3_convert_v11_to_v23
    id3_convert_v11_to_v24
    id3_convert_v20_to_v10
    id3_convert_v20_to_v11
    id3_convert_v23_to_v10
    id3_convert_v23_to_v11
    id3_convert_v24_to_v10
    id3_convert_v24_to_v11
)

foreach(APP ${APPLICATIONS})
    add_executable(${APP} ${APP}.cpp)
    target_link_libraries(${APP} PRIVATE ID3::shared)
endforeach()

# Installation
install(TARGETS ${APPLICATIONS}
    RUNTIME DESTINATION binary
)
