/**
 * @file id3_scanner.cpp
 * @brief Scan MP3 files and display ID3 tag versions using the ID3 library
 */

#include "../source/MP3File.hpp"
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

// Forward declarations
bool isMp3File(const std::string& filename);
std::string getID3Versions(const MP3File& mp3);
void scanDirectory(const fs::path& dirPath);

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    
    // Obter argumentos em Unicode via WinAPI
    LPWSTR* szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (szArglist == NULL || nArgs < 2) {
        std::cout << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    }
    
    fs::path dirPath(szArglist[1]);
    LocalFree(szArglist);
    #else
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    }
    fs::path dirPath(argv[1]);
    #endif
    
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        std::cerr << "Error: Directory does not exist." << std::endl;
        return 1;
    }
    
    scanDirectory(dirPath);
    
    return 0;
}

// Implementações

bool isMp3File(const std::string& filename) {
    if (filename.length() < 4) return false;
    std::string ext = filename.substr(filename.length() - 4);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".mp3";
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

void scanDirectory(const fs::path& dirPath) {
    std::cout << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "    ID3 Tag Scanner Application" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Scanning directory: " << dirPath.string() << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    int fileCount = 0;
    int mp3Count = 0;
    
    try {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                
                if (isMp3File(filename)) {
                    fileCount++;
                    
                    MP3File mp3;
                    // Use u8string() para garantir UTF-8 correto
                    std::string path_utf8 = entry.path().u8string();
                    if (mp3.load(path_utf8.c_str())) {
                        std::string versions = getID3Versions(mp3);
                        std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                                  << filename << " = " << versions << std::endl;
                        mp3Count++;
                    } else {
                        std::cout << std::setw(2) << std::setfill('0') << fileCount << ") "
                                  << filename << " = Failed to load" << std::endl;
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return;
    }
    
    std::cout << std::string(80, '-') << std::endl;
    std::cout << "Total files: " << fileCount << std::endl;
    std::cout << "MP3 files found: " << mp3Count << std::endl;
}
