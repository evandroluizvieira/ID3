/**
 * @file id3_full_process.cpp
 * @brief Full pipeline using the ID3 library
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
fs::path processDirectory(const fs::path& sourcePath, bool removeV1);
std::string toAnsiPath(const fs::path& path);

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    
    // Obter argumentos em Unicode via WinAPI
    LPWSTR* szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (szArglist == NULL || nArgs < 2) {
        std::cout << "Usage: " << argv[0] << " <source_directory>" << std::endl;
        return 1;
    }
    
    fs::path sourcePath(szArglist[1]);
    LocalFree(szArglist);
    #else
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <source_directory>" << std::endl;
        return 1;
    }
    fs::path sourcePath(argv[1]);
    #endif
    
    std::cout << "=====================================" << std::endl;
    std::cout << "  Full ID3 Removal Pipeline" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // 1. Scan original
    std::cout << std::endl;
    std::cout << "INITIAL SCAN" << std::endl;
    scanDirectory(sourcePath);
    
    // 2. Remove ID3v1 from original
    fs::path noV1Path = processDirectory(sourcePath, true);
    
    // 3. Scan No ID3v1
    scanDirectory(noV1Path);
    
    // 4. Remove ID3v2 from original (not from No ID3v1!)
    fs::path noV2Path = processDirectory(sourcePath, false);
    
    // 5. Scan No ID3v2
    scanDirectory(noV2Path);
    
    std::cout << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "  Pipeline Complete!" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    return 0;
}

// Implementações

std::string toAnsiPath(const fs::path& path) {
    #ifdef _WIN32
    std::wstring wpath = path.wstring();
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wpath.c_str(), -1, NULL, 0, NULL, NULL);
    std::string ansiPath(size_needed - 1, 0);
    WideCharToMultiByte(CP_ACP, 0, wpath.c_str(), -1, &ansiPath[0], size_needed, NULL, NULL);
    return ansiPath;
    #else
    return path.string();
    #endif
}

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
    std::cout << "SCANNING: " << dirPath.string() << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    int fileCount = 0;
    
    try {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                
                if (isMp3File(filename)) {
                    fileCount++;
                    
                    MP3File mp3;
                    std::string path_utf8 = entry.path().u8string();
                    if (mp3.load(path_utf8.c_str())) {
                        std::string versions = getID3Versions(mp3);
                        std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                                  << filename << " = " << versions << std::endl;
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
    std::cout << "Total MP3 files: " << fileCount << std::endl;
}

fs::path processDirectory(const fs::path& sourcePath, bool removeV1) {
    if (!fs::exists(sourcePath) || !fs::is_directory(sourcePath)) {
        std::cerr << "Error: Source directory does not exist." << std::endl;
        return sourcePath;
    }
    
    std::string dirName = sourcePath.filename().string();
    
    if (dirName.length() > 12 && dirName.substr(dirName.length() - 12) == " - No ID3v1")
        dirName = dirName.substr(0, dirName.length() - 12);
    if (dirName.length() > 12 && dirName.substr(dirName.length() - 12) == " - No ID3v2")
        dirName = dirName.substr(0, dirName.length() - 12);
    
    fs::path parentPath = sourcePath.parent_path();
    std::string suffix = removeV1 ? " - No ID3v1" : " - No ID3v2";
    fs::path outputPath = parentPath / (dirName + suffix);
    
    // Se o diretório existe, remove completamente
    if (fs::exists(outputPath)) {
        try {
            fs::remove_all(outputPath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Warning: Could not remove existing directory: " << e.what() << std::endl;
            std::cerr << "Attempting to delete files individually..." << std::endl;
            // Tenta remover arquivos individualmente
            try {
                for (const auto& entry : fs::recursive_directory_iterator(outputPath)) {
                    try {
                        if (entry.is_regular_file()) {
                            fs::remove(entry.path());
                        }
                    } catch (...) {}
                }
                // Tenta remover o diretório novamente
                fs::remove_all(outputPath);
            } catch (...) {
                std::cerr << "Error: Could not clean output directory. Please close any open files." << std::endl;
            }
        }
    }
    
    // Cria o diretório de saída
    try {
        fs::create_directories(outputPath);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: Could not create output directory: " << e.what() << std::endl;
        return sourcePath;
    }
    
    std::cout << std::endl;
    if (removeV1) {
        std::cout << "REMOVING ID3v1 TAGS" << std::endl;
    } else {
        std::cout << "REMOVING ID3v2 TAGS" << std::endl;
    }
    std::cout << "Source: " << sourcePath.string() << std::endl;
    std::cout << "Output: " << outputPath.string() << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    int mp3sProcessed = 0;
    int removedCount = 0;
    
    try {
        for (const auto& entry : fs::directory_iterator(sourcePath)) {
            std::string filename = entry.path().filename().string();
            
            if (entry.is_regular_file()) {
                if (isMp3File(filename)) {
                    mp3sProcessed++;
                    
                    MP3File mp3;
                    std::string path_utf8 = entry.path().u8string();
                    if (mp3.load(path_utf8.c_str())) {
                        bool hasTag = false;
                        std::string action;
                        
                        if (removeV1) {
                            hasTag = mp3.hasID3v10() || mp3.hasID3v11();
                            if (hasTag) {
                                ID3 id3 = mp3.getID3();
                                id3.v1.clear();
                                mp3.setID3(id3);
                                removedCount++;
                                action = "ID3v1 removed";
                            } else {
                                action = "No ID3v1 tag (copied)";
                            }
                        } else {
                            hasTag = mp3.hasID3v20() || mp3.hasID3v23() || mp3.hasID3v24();
                            if (hasTag) {
                                ID3 id3 = mp3.getID3();
                                id3.v2.clear();
                                mp3.setID3(id3);
                                removedCount++;
                                action = "ID3v2 removed";
                            } else {
                                action = "No ID3v2 tag (copied)";
                            }
                        }
                        
                        std::cout << std::setw(3) << std::setfill('0') << mp3sProcessed << ") " 
                                  << filename << " = " << action << std::endl;
                        
                        fs::path outputFile = outputPath / filename;
                        std::string output_utf8 = outputFile.u8string();
                        if (!mp3.save(output_utf8.c_str())) {
                            std::cerr << "    Failed to save: " << filename << std::endl;
                        }
                    } else {
                        std::cout << std::setw(3) << std::setfill('0') << mp3sProcessed << ") " 
                                  << filename << " = Failed to load" << std::endl;
                    }
                } else {
                    fs::path outputFile = outputPath / filename;
                    try {
                        // Se o arquivo existe, tenta remover primeiro
                        if (fs::exists(outputFile)) {
                            try {
                                fs::remove(outputFile);
                            } catch (...) {
                                // Ignora erro de remoção, tentará sobrescrever
                            }
                        }
                        fs::copy_file(entry.path(), outputFile, fs::copy_options::overwrite_existing);
                    } catch (const fs::filesystem_error& e) {
                        std::cerr << "Error copying file " << filename << ": " << e.what() << std::endl;
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return sourcePath;
    }
    
    std::cout << std::string(80, '-') << std::endl;
    std::cout << "MP3 files processed: " << mp3sProcessed << std::endl;
    
    return outputPath;
}
