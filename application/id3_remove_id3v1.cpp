/**
 * @file id3_remove_id3v1.cpp
 * @brief Remove ID3v1 tags from MP3 files using the ID3 library
 */

#include <MP3File>
#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace fs = std::filesystem;

// Forward declarations
bool isMp3File(const std::string& filename);
void processDirectory(const fs::path& sourcePath);

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
    
    processDirectory(sourcePath);
    
    return 0;
}

// Implementações

bool isMp3File(const std::string& filename) {
    if (filename.length() < 4) return false;
    std::string ext = filename.substr(filename.length() - 4);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".mp3";
}

void processDirectory(const fs::path& sourcePath) {
    if (!fs::exists(sourcePath) || !fs::is_directory(sourcePath)) {
        std::cerr << "Error: Source directory does not exist." << std::endl;
        return;
    }
    
    std::string dirName = sourcePath.filename().string();
    if (dirName.length() > 12 && dirName.substr(dirName.length() - 12) == " - No ID3v1")
        dirName = dirName.substr(0, dirName.length() - 12);
    
    fs::path parentPath = sourcePath.parent_path();
    fs::path outputPath = parentPath / (dirName + " - No ID3v1");
    
    try {
        if (fs::exists(outputPath)) fs::remove_all(outputPath);
        fs::create_directories(outputPath);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating output directory: " << e.what() << std::endl;
        return;
    }
    
    std::cout << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "  Remove ID3v1 Tags" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "Source: " << sourcePath.string() << std::endl;
    std::cout << "Output: " << outputPath.string() << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    int filesProcessed = 0;
    int mp3sProcessed = 0;
    int removedCount = 0;
    
    try {
        for (const auto& entry : fs::directory_iterator(sourcePath)) {
            std::string filename = entry.path().filename().string();
            
            if (entry.is_regular_file()) {
                filesProcessed++;
                
                if (isMp3File(filename)) {
                    mp3sProcessed++;
                    
                    MP3File mp3;
                    std::string path_utf8 = entry.path().u8string();
                    if (mp3.load(path_utf8.c_str())) {
                        bool hasTag = mp3.hasID3v10() || mp3.hasID3v11();
                        
                        if (hasTag) {
                            ID3 id3 = mp3.getID3();
                            id3.v1.clear();
                            mp3.setID3(id3);
                            removedCount++;
                            
                            std::cout << std::setw(3) << std::setfill('0') << mp3sProcessed << ") " 
                                      << filename << " = ID3v1 removed" << std::endl;
                        } else {
                            std::cout << std::setw(3) << std::setfill('0') << mp3sProcessed << ") " 
                                      << filename << " = No ID3v1 tag (copied)" << std::endl;
                        }
                        
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
                        fs::copy_file(entry.path(), outputFile, fs::copy_options::overwrite_existing);
                    } catch (const fs::filesystem_error& e) {
                        std::cerr << "Error copying file " << filename << ": " << e.what() << std::endl;
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return;
    }
    
    std::cout << std::string(80, '-') << std::endl;
    std::cout << "Total files: " << filesProcessed << std::endl;
    std::cout << "MP3 files: " << mp3sProcessed << std::endl;
    std::cout << "ID3v1 tags removed: " << removedCount << std::endl;
    std::cout << "=====================================" << std::endl;
}
