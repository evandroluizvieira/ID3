/**
 * @file id3_scan_mp3.cpp
 * @brief Scan a single MP3 file and display its ID3 tags in detail.
 *
 * Usage: id3_scan_mp3.exe <mp3_file>
 *
 * Shows: versions present, and details of Track, Title, Artist, Album, Year
 *        from both v1 (if exists) and v2 (if exists).
 */

#include <ID3>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace fs = std::filesystem;

void scanMp3File(const fs::path& mp3Path);
std::string getID3Versions(const MP3File& mp3);

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);

    LPWSTR* szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (szArglist == NULL || nArgs < 2) {
        std::cout << "Usage: " << argv[0] << " <mp3_file>" << std::endl;
        LocalFree(szArglist);
        return 1;
    }

    fs::path mp3Path(szArglist[1]);
    LocalFree(szArglist);
    #else
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <mp3_file>" << std::endl;
        return 1;
    }
    fs::path mp3Path(argv[1]);
    #endif

    if (!fs::exists(mp3Path) || !fs::is_regular_file(mp3Path)) {
        std::cerr << "Error: File does not exist." << std::endl;
        return 1;
    }

    scanMp3File(mp3Path);

    return 0;
}

std::string getID3Versions(const MP3File& mp3) {
    std::vector<std::string> versions;

    if (mp3.hasID3v10()) versions.push_back("ID3v1.0");
    if (mp3.hasID3v11()) versions.push_back("ID3v1.1");
    if (mp3.hasID3v20()) versions.push_back("ID3v2.0");
    if (mp3.hasID3v23()) versions.push_back("ID3v2.3");
    if (mp3.hasID3v24()) versions.push_back("ID3v2.4");

    if (versions.empty()) return "No ID3 tags found";

    std::string result;
    for (size_t i = 0; i < versions.size(); ++i) {
        result += versions[i];
        if (i < versions.size() - 1) result += ", ";
    }
    return result;
}

void scanMp3File(const fs::path& mp3Path) {
    std::string path_utf8 = mp3Path.u8string();
    std::string filename = mp3Path.filename().string();

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "         ID3 Scan MP3" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "File: " << filename << std::endl;
    std::cout << "Path: " << mp3Path.string() << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    MP3File mp3;
    if (!mp3.load(path_utf8.c_str())) {
        std::cout << "Status: Failed to load MP3 file" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        return;
    }

    // Show versions
    std::string versions = getID3Versions(mp3);
    std::cout << "Tags:  " << versions << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    // Get all ID3 data
    ID3 id3 = mp3.getID3();

    // ---- ID3v1 details ----
    bool hasV1 = mp3.hasID3v10() || mp3.hasID3v11();

    if (hasV1) {
        bool isV11 = mp3.hasID3v11();
        std::cout << "ID3v1." << (isV11 ? "1" : "0") << " details:" << std::endl;

        if (isV11 && id3.v1.v11) {
            std::cout << "  Track:  " << (int)id3.v1.v11->getTrack() << std::endl;
            std::cout << "  Title:  " << id3.v1.v11->getTitle() << std::endl;
            std::cout << "  Artist: " << id3.v1.v11->getArtist() << std::endl;
            std::cout << "  Album:  " << id3.v1.v11->getAlbum() << std::endl;
            std::cout << "  Year:   " << id3.v1.v11->getYear() << std::endl;
        } else if (id3.v1.v10) {
            std::cout << "  Title:  " << id3.v1.v10->getTitle() << std::endl;
            std::cout << "  Artist: " << id3.v1.v10->getArtist() << std::endl;
            std::cout << "  Album:  " << id3.v1.v10->getAlbum() << std::endl;
            std::cout << "  Year:   " << id3.v1.v10->getYear() << std::endl;
        }
    }

    // ---- ID3v2 details ----
    bool hasV2 = mp3.hasID3v20() || mp3.hasID3v23() || mp3.hasID3v24();

    if (hasV2) {
        std::cout << std::endl;
        if (mp3.hasID3v24() && id3.v2.v24) {
            std::cout << "ID3v2.4 details:" << std::endl;
            std::cout << "  Track:  " << id3.v2.v24->getTrack() << std::endl;
            std::cout << "  Title:  " << id3.v2.v24->getTitle() << std::endl;
            std::cout << "  Artist: " << id3.v2.v24->getArtist() << std::endl;
            std::cout << "  Album:  " << id3.v2.v24->getAlbum() << std::endl;
            std::cout << "  Year:   " << id3.v2.v24->getYear() << std::endl;
        } else if (mp3.hasID3v23() && id3.v2.v23) {
            std::cout << "ID3v2.3 details:" << std::endl;
            std::cout << "  Track:  " << id3.v2.v23->getTrack() << std::endl;
            std::cout << "  Title:  " << id3.v2.v23->getTitle() << std::endl;
            std::cout << "  Artist: " << id3.v2.v23->getArtist() << std::endl;
            std::cout << "  Album:  " << id3.v2.v23->getAlbum() << std::endl;
            std::cout << "  Year:   " << id3.v2.v23->getYear() << std::endl;
        } else if (mp3.hasID3v20() && id3.v2.v20) {
            std::cout << "ID3v2.0 details:" << std::endl;
            std::cout << "  Track:  " << (int)id3.v2.v20->getTrack() << std::endl;
            std::cout << "  Title:  " << id3.v2.v20->getTitle() << std::endl;
            std::cout << "  Artist: " << id3.v2.v20->getArtist() << std::endl;
            std::cout << "  Album:  " << id3.v2.v20->getAlbum() << std::endl;
            std::cout << "  Year:   " << id3.v2.v20->getYear() << std::endl;
        }
    }

    if (!hasV1 && !hasV2) {
        std::cout << "No tag details to display." << std::endl;
    }

    std::cout << std::string(50, '=') << std::endl;
}
