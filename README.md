# ID3

[![C++](https://img.shields.io/badge/C++-17-004488)](https://cplusplus.com/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![MSYS64](https://img.shields.io/badge/Compiler-MSYS64_MinGW64-blue)](https://www.msys2.org/)

## Overview
ID3 is a C++ library for reading, writing, and manipulating ID3 tags in MP3 files. It provides comprehensive support for both ID3v1 (v1.0, v1.1) and ID3v2 (v2.0, v2.3, v2.4) tag formats, enabling full control over MP3 metadata.

The library includes a complete set of command-line applications for common ID3 tag operations, making it easy to scan, remove, and convert tags between different versions.

The library is open source and licensed under the MIT license.

## Features
- **Multi-Version Support**: Full compatibility with ID3v1.0, v1.1, v2.0, v2.3, and v2.4
- **Tag Reading & Writing**: Extract and modify all standard ID3 fields (title, artist, album, year, genre, track, comment)
- **Tag Removal**: Selective removal of ID3v1 or ID3v2 tags independently
- **Version Conversion**: Convert between any ID3 version with automatic field mapping
- **UTF-8 Support**: Handles file paths with special characters via Windows Unicode APIs
- **In-Memory Operations**: Efficient binary manipulation without temporary files
- **MP3 Frame Parsing**: Analyze MP3 frame structure and validate audio data
- **16 Command-Line Tools**: Ready-to-use applications for all common operations

## Installation
To get started, clone the repository:
```bash
git clone https://github.com/evandroluizvieira/ID3.git
```

## Build

### Prerequisites
- MSYS64 with MinGW64 toolchain
- GCC with C++17 support
- Windows 10/11 (for UTF-8 path support)

### Compile Library
```bash
g++ -std=c++17 -O2 -DNDEBUG -c source/*.cpp
ar rcs libid3.a *.o
```

### Compile Applications
```bash
g++ -std=c++17 -O2 -DNDEBUG \
    application/id3_scan_id3.cpp \
    source/File.cpp source/MP3File.cpp source/MP3.cpp \
    source/MP3Frame.cpp source/MP3FrameHeader.cpp \
    source/ID3.cpp source/ID3v1.cpp source/ID3v10.cpp source/ID3v11.cpp \
    source/ID3v2.cpp source/ID3v20.cpp source/ID3v23.cpp source/ID3v24.cpp \
    -o id3_scan_id3.exe
```

## Library Usage

### Basic Tag Reading
```cpp
#include "MP3File.hpp"

MP3File mp3;
mp3.load("song.mp3");

if (mp3.hasID3v23()) {
    ID3 id3 = mp3.getID3();
    std::string title = id3.v2.v23->getTitle();
    std::string artist = id3.v2.v23->getArtist();
    std::string album = id3.v2.v23->getAlbum();
}
```

### Tag Writing
```cpp
MP3File mp3;
mp3.load("song.mp3");

ID3 id3 = mp3.getID3();
if (!id3.v2.v23) {
    id3.v2.v23 = new ID3v23();
}

id3.v2.v23->setTitle("New Title");
id3.v2.v23->setArtist("New Artist");
id3.v2.v23->setAlbum("New Album");
id3.v2.v23->setYear("2026");

mp3.setID3(id3);
mp3.save("song.mp3");
```

### Version Conversion
```cpp
// Convert ID3v1.1 to ID3v2.3
MP3File mp3;
mp3.load("song.mp3");

if (mp3.hasID3v11()) {
    ID3 id3 = mp3.getID3();
    
    // Extract v1.1 data
    std::string title = id3.v1.v11->getTitle();
    std::string artist = id3.v1.v11->getArtist();
    uint8_t track = id3.v1.v11->getTrack();
    
    // Create v2.3 tag
    if (!id3.v2.v23) id3.v2.v23 = new ID3v23();
    id3.v2.v23->setTitle(title);
    id3.v2.v23->setArtist(artist);
    if (track > 0) id3.v2.v23->setTrack(std::to_string(track));
    
    mp3.setID3(id3);
    mp3.save("song.mp3");
}
```

## Applications

### Core Utilities
- **id3_scan_id3** - Display ID3 tag versions in directory
- **id3_remove_id3v1** - Remove only ID3v1 tags
- **id3_remove_id3v2** - Remove only ID3v2 tags  
- **id3_full_process** - Complete pipeline (scan → remove v1 → remove v2 → scan)

### Version Conversions

#### ID3v1 to ID3v2
- **id3_convert_v10_to_v20** - Convert v1.0 → v2.0
- **id3_convert_v10_to_v23** - Convert v1.0 → v2.3
- **id3_convert_v10_to_v24** - Convert v1.0 → v2.4
- **id3_convert_v11_to_v20** - Convert v1.1 → v2.0
- **id3_convert_v11_to_v23** - Convert v1.1 → v2.3
- **id3_convert_v11_to_v24** - Convert v1.1 → v2.4

#### ID3v2 to ID3v1
- **id3_convert_v20_to_v10** - Convert v2.0 → v1.0
- **id3_convert_v20_to_v11** - Convert v2.0 → v1.1
- **id3_convert_v23_to_v10** - Convert v2.3 → v1.0
- **id3_convert_v23_to_v11** - Convert v2.3 → v1.1
- **id3_convert_v24_to_v10** - Convert v2.4 → v1.0
- **id3_convert_v24_to_v11** - Convert v2.4 → v1.1

### Application Usage
All applications follow the same pattern:
```bash
# Scan directory
id3_scan_id3.exe "C:\Music\My Album"

# Remove ID3v1 tags
id3_remove_id3v1.exe "C:\Music\My Album"

# Convert v1.1 to v2.3
id3_convert_v11_to_v23.exe "C:\Music\My Album"
```

**Output:** Creates new directory with suffix describing the operation
- `My Album - No ID3v1/` - After v1 removal
- `My Album - v1.1 to v2.3/` - After conversion

## Project Structure
```
ID3/
├── source/           # Library source files
│   ├── File.cpp/hpp          # Binary file manipulation
│   ├── MP3File.cpp/hpp       # MP3 file I/O and tag handling
│   ├── MP3.cpp/hpp           # MP3 structure analysis
│   ├── MP3Frame.cpp/hpp      # Frame parsing
│   ├── MP3FrameHeader.cpp/hpp # Frame header decoding
│   ├── ID3.cpp/hpp           # ID3 tag container
│   ├── ID3v1.cpp/hpp         # ID3v1 base
│   ├── ID3v10.cpp/hpp        # ID3 version 1.0
│   ├── ID3v11.cpp/hpp        # ID3 version 1.1
│   ├── ID3v2.cpp/hpp         # ID3v2 base
│   ├── ID3v20.cpp/hpp        # ID3 version 2.0
│   ├── ID3v23.cpp/hpp        # ID3 version 2.3
│   └── ID3v24.cpp/hpp        # ID3 version 2.4
├── application/      # Command-line tools (16 applications)
├── include/          # Public API headers
├── test/             # Unit tests (Google Test)
├── .vscode/          # VS Code build/debug configurations
└── README.md
```

## Technical Details

### ID3v1 Format
- Fixed 128-byte structure at end of file
- **v1.0**: Title, Artist, Album, Year, Comment (30 chars each), Genre (1 byte)
- **v1.1**: Same as v1.0 but Comment is 29 chars + Track number (1 byte)

### ID3v2 Format
- Variable-size structure at beginning of file
- **v2.0**: 3-character frame IDs (e.g., "TT2" for title)
- **v2.3**: 4-character frame IDs (e.g., "TIT2" for title)
- **v2.4**: Extended header, unsynchronization, footer support

### Conversion Mapping
| Field   | ID3v1.0/v1.1 | ID3v2.x |
|---------|--------------|---------|
| Title   | 30 chars     | Unlimited string |
| Artist  | 30 chars     | Unlimited string |
| Album   | 30 chars     | Unlimited string |
| Year    | 4 chars      | String (YYYY format) |
| Comment | 30/29 chars  | Unlimited string |
| Track   | v1.1 only (uint8_t) | String (e.g., "01", "12/15") |
| Genre   | Enum (0-255) | String (e.g., "Rock") |

## Development

### Branch Structure
- **master**: Stable releases
- **feature/id3v2-improvements**: ID3v2 class enhancements
- **feature/mp3-frame-classes**: MP3 frame parsing
- **feature/id3-applications**: Command-line tools
- **feature/documentation-and-tests**: Docs and unit tests

### Contributing
Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Author
Evandro Luiz Vieira - [evandroluizvieira@hotmail.com](mailto:evandroluizvieira@hotmail.com)
