/**
 * @file id3_convert_v24_to_v10.cpp
 * @brief Convert ID3v2.4 tags to ID3v1.0 tags
 */

#include <MP3File>
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

bool isMp3File(const std::string& filename);
std::string getID3Versions(const MP3File& mp3);
void scanDirectory(const fs::path& dirPath);
fs::path processDirectory(const fs::path& sourcePath);

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    
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
    std::cout << "  Convert ID3v2.4 to ID3v1.0" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    std::cout << std::endl << "INITIAL SCAN" << std::endl;
    scanDirectory(sourcePath);
    
    fs::path convertedPath = processDirectory(sourcePath);
    
    scanDirectory(convertedPath);
    
    std::cout << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "  Conversion Complete!" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    return 0;
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
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        std::cerr << "Error: Directory does not exist: " << dirPath << std::endl;
        return;
    }
    
    std::cout << std::endl << "Scanning: " << dirPath << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    std::vector<fs::directory_entry> mp3Files;
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file() && isMp3File(entry.path().filename().string())) {
            mp3Files.push_back(entry);
        }
    }
    
    std::sort(mp3Files.begin(), mp3Files.end(), 
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename().string() < b.path().filename().string();
        });
    
    int fileCount = 0;
    for (const auto& entry : mp3Files) {
        fileCount++;
        std::string path_utf8 = entry.path().u8string();
        std::string filename = entry.path().filename().string();
        
        MP3File mp3;
        if (!mp3.load(path_utf8.c_str())) {
            std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                      << filename << " = Error loading" << std::endl;
            continue;
        }
        
        std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                  << filename << " = " << getID3Versions(mp3) << std::endl;
    }
    
    std::cout << "Total MP3 files: " << fileCount << std::endl;
}

fs::path processDirectory(const fs::path& sourcePath) {
    std::string dirname = sourcePath.filename().string();
    size_t pos = dirname.find(" - v2.4 to v1.0");
    if (pos != std::string::npos) dirname = dirname.substr(0, pos);
    dirname += " - v2.4 to v1.0";
    
    fs::path targetPath = sourcePath.parent_path() / dirname;
    
    std::cout << std::endl << "Converting: " << sourcePath << std::endl;
    std::cout << "Target: " << targetPath << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    if (fs::exists(targetPath)) {
        std::error_code ec;
        fs::remove_all(targetPath, ec);
        if (ec && fs::exists(targetPath)) {
            for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
                if (entry.is_regular_file()) fs::remove(entry.path(), ec);
            }
            fs::remove_all(targetPath, ec);
        }
    }
    
    fs::create_directories(targetPath);
    
    std::vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(sourcePath)) {
        if (entry.is_regular_file()) entries.push_back(entry);
    }
    
    std::sort(entries.begin(), entries.end(), 
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename().string() < b.path().filename().string();
        });
    
    int fileCount = 0, convertedCount = 0;
    
    for (const auto& entry : entries) {
        std::string filename = entry.path().filename().string();
        fs::path targetFile = targetPath / filename;
        
        if (fs::exists(targetFile)) {
            std::error_code ec;
            fs::remove(targetFile, ec);
        }
        
        if (!isMp3File(filename)) {
            std::error_code ec;
            fs::copy_file(entry.path(), targetFile, fs::copy_options::overwrite_existing, ec);
            continue;
        }
        
        fileCount++;
        std::string path_utf8 = entry.path().u8string();
        
        MP3File mp3;
        if (!mp3.load(path_utf8.c_str())) {
            std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                      << filename << " = Error loading" << std::endl;
            continue;
        }
        
        if (mp3.hasID3v24()) {
            ID3 id3 = mp3.getID3();
            
            std::string title = id3.v2.v24->getTitle();
            std::string artist = id3.v2.v24->getArtist();
            std::string album = id3.v2.v24->getAlbum();
            std::string year = id3.v2.v24->getYear();
            std::string comment = id3.v2.v24->getComment();
            
            if (!id3.v1.v10) id3.v1.v10 = new ID3v10();
            
            id3.v1.v10->setTitle(title);
            id3.v1.v10->setArtist(artist);
            id3.v1.v10->setAlbum(album);
            id3.v1.v10->setYear(year);
            id3.v1.v10->setComment(comment);
            id3.v1.v10->setGenre(ID3v10::Other);
            
            mp3.setID3(id3);
            std::string targetPath_utf8 = targetFile.u8string();
            if (mp3.save(targetPath_utf8.c_str())) {
                convertedCount++;
                std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                          << filename << " = Converted v2.4 to v1.0" << std::endl;
            } else {
                std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                          << filename << " = Error saving" << std::endl;
            }
        } else {
            std::string targetPath_utf8 = targetFile.u8string();
            if (mp3.save(targetPath_utf8.c_str())) {
                std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                          << filename << " = Copied (no v2.4)" << std::endl;
            } else {
                std::cout << std::setw(2) << std::setfill('0') << fileCount << ") " 
                          << filename << " = Error saving" << std::endl;
            }
        }
    }
    
    std::cout << "Processed: " << fileCount << " MP3 files" << std::endl;
    std::cout << "Converted: " << convertedCount << " files" << std::endl;
    
    return targetPath;
}
