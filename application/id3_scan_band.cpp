/**
 * @file id3_scan_band.cpp
 * @brief Scan all albums of a band and call id3_scan_mp3 for each MP3 file.
 *
 * Usage: id3_scan_band.exe <band_path>
 *
 * Iterates over album subfolders (YYYY - Album Name) and for each MP3
 * file found, calls: id3_scan_mp3.exe <full_mp3_path>
 */

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace fs = std::filesystem;

bool isMp3File(const std::string& filename);

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);

    LPWSTR* szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (szArglist == NULL || nArgs < 2) {
        std::cout << "Usage: " << argv[0] << " <band_path>" << std::endl;
        std::cout << "Scans all albums of a band and calls id3_scan_mp3 for each MP3." << std::endl;
        LocalFree(szArglist);
        return 1;
    }

    fs::path bandPath(szArglist[1]);
    LocalFree(szArglist);
    #else
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <band_path>" << std::endl;
        return 1;
    }
    fs::path bandPath(argv[1]);
    #endif

    if (!fs::exists(bandPath) || !fs::is_directory(bandPath)) {
        std::cerr << "Error: Band directory does not exist." << std::endl;
        return 1;
    }

    std::string bandName = bandPath.filename().string();

    std::cout << "========================================" << std::endl;
    std::cout << "       ID3 Scan Band" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Band: " << bandName << std::endl;
    std::cout << "Path: " << bandPath.string() << std::endl;
    std::cout << "========================================" << std::endl;

    // Collect album subfolders
    std::vector<fs::directory_entry> albums;
    for (const auto& entry : fs::directory_iterator(bandPath)) {
        if (entry.is_directory()) {
            albums.push_back(entry);
        }
    }
    std::sort(albums.begin(), albums.end(),
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename().string() < b.path().filename().string();
        });

    if (albums.empty()) {
        std::cout << "No album folders found." << std::endl;
        return 0;
    }

    int totalMp3s = 0;
    int albumIndex = 0;

    for (const auto& albumEntry : albums) {
        fs::path albumPath = albumEntry.path();
        std::string albumName = albumPath.filename().string();

        // Collect MP3s inside album folder
        std::vector<fs::directory_entry> mp3Files;
        for (const auto& file : fs::directory_iterator(albumPath)) {
            if (file.is_regular_file()) {
                std::string fname = file.path().filename().string();
                if (isMp3File(fname)) {
                    mp3Files.push_back(file);
                }
            }
        }

        if (mp3Files.empty()) continue;

        albumIndex++;

        std::cout << std::endl;
        std::cout << "[" << albumIndex << "] " << albumName
                  << " (" << mp3Files.size() << " MP3s)" << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        std::sort(mp3Files.begin(), mp3Files.end(),
            [](const fs::directory_entry& a, const fs::directory_entry& b) {
                return a.path().filename().string() < b.path().filename().string();
            });

        for (const auto& mp3Entry : mp3Files) {
            fs::path mp3Path = mp3Entry.path();

            // Build command to call id3_scan_mp3.exe
            #ifdef _WIN32
            std::string cmd = "id3_scan_mp3.exe \"" + mp3Path.string() + "\"";
            #else
            std::string cmd = "./id3_scan_mp3 \"" + mp3Path.string() + "\"";
            #endif

            int result = std::system(cmd.c_str());

            if (result != 0) {
                std::cout << "  [Error] Failed to scan: " << mp3Path.filename().string() << std::endl;
            }

            totalMp3s++;
        }
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Scan complete." << std::endl;
    std::cout << "Albums: " << albumIndex << std::endl;
    std::cout << "Total MP3s scanned: " << totalMp3s << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

bool isMp3File(const std::string& filename) {
    if (filename.length() < 4) return false;
    std::string ext = filename.substr(filename.length() - 4);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".mp3";
}
