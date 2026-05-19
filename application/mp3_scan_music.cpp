/**
 * @file mp3_scan_music.cpp
 * @brief Scan a single MP3 file and display audio/technical information.
 *
 * Usage: mp3_scan_music.exe <mp3_file>
 *
 * Shows:
 *   - File size, audio data size
 *   - MPEG version, Layer, Bitrate, Sample rate, Channels (Mono/Stereo)
 *   - Duration, Number of frames
 *   - Presence of ID3 tags (without detail)
 */

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <cstdint>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace fs = std::filesystem;

// MPEG version names
const char* mpegVersionName(uint8_t versionBits) {
    switch (versionBits) {
        case 0: return "MPEG 2.5";
        case 2: return "MPEG 2.0";
        case 3: return "MPEG 1.0";
        default: return "Reserved";
    }
}

// Layer names
const char* layerName(uint8_t layerBits) {
    switch (layerBits) {
        case 1: return "Layer III";
        case 2: return "Layer II";
        case 3: return "Layer I";
        default: return "Reserved";
    }
}

// Channel mode names
const char* channelModeName(uint8_t mode) {
    switch (mode) {
        case 0: return "Stereo";
        case 1: return "Joint Stereo";
        case 2: return "Dual Channel";
        case 3: return "Mono";
        default: return "Unknown";
    }
}

// Bitrate table [mpegVersion][layer][bitrateIndex]
// mpegVersion: 0=MPEG1, 1=MPEG2/2.5
// layer: 0=LayerI, 1=LayerII, 2=LayerIII
const uint16_t bitrateTable[2][3][16] = {
    { // MPEG1
        {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0},// Layer I
        {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 0},   // Layer II
        {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0}     // Layer III
    },
    { // MPEG2/2.5
        {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0},   // Layer I
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0},        // Layer II
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0}         // Layer III
    }
};

// Sample rate table [mpegVersion][sampleRateIndex]
const uint32_t sampleRateTable[4][4] = {
    {44100, 48000, 32000, 0},  // MPEG1
    {22050, 24000, 16000, 0},  // MPEG2
    {11025, 12000, 8000, 0},   // MPEG2.5
    {0, 0, 0, 0}               // Reserved
};

// Samples per frame table [mpegVersion][layer]
const uint16_t samplesPerFrameTable[4][4] = {
    {0, 384, 1152, 1152},  // MPEG1
    {0, 384, 1152, 576},   // MPEG2
    {0, 384, 1152, 576},   // MPEG2.5
    {0, 0, 0, 0}           // Reserved
};

struct Mp3AudioInfo {
    int frameCount = 0;
    int mpegVersionBits = 0;
    int layerBits = 0;
    int bitrateIndex = 0;
    int sampleRateIndex = 0;
    bool hasPadding = false;
    int channelMode = 0;
    uint16_t bitrate = 0;
    uint32_t sampleRate = 0;
    double totalDurationMs = 0.0;
    uint64_t audioDataSize = 0;
    bool hasVBR = false;
    bool valid = false;
};

Mp3AudioInfo scanAudioFrames(const std::string& filepath);

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

    std::string filename = mp3Path.filename().string();
    uint64_t fileSize = fs::file_size(mp3Path);

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "         MP3 Scan Music" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "File: " << filename << std::endl;
    std::cout << "Path: " << mp3Path.string() << std::endl;
    std::cout << "Size: " << fileSize << " bytes";
    if (fileSize > 1024 * 1024) {
        std::cout << " (" << std::fixed << std::setprecision(1)
                  << (double)fileSize / (1024.0 * 1024.0) << " MB)";
    } else if (fileSize > 1024) {
        std::cout << " (" << (fileSize / 1024) << " KB)";
    }
    std::cout << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    // Scan audio frames
    Mp3AudioInfo info = scanAudioFrames(mp3Path.string());

    if (!info.valid) {
        std::cout << "Could not parse MPEG audio frames." << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        return 0;
    }

    // Display results
    std::cout << "Audio Stream:" << std::endl;

    int verIdx;
    switch (info.mpegVersionBits) {
        case 3: verIdx = 0; break;  // MPEG1
        case 2: verIdx = 1; break;  // MPEG2
        case 0: verIdx = 2; break;  // MPEG2.5
        default: verIdx = 3;
    }

    std::cout << "  MPEG Version:  " << mpegVersionName(info.mpegVersionBits) << std::endl;
    std::cout << "  Layer:         " << layerName(info.layerBits) << std::endl;
    std::cout << "  Bitrate:       " << info.bitrate << " kbps";

    if (info.hasVBR) {
        std::cout << " (VBR - average from " << info.frameCount << " frames)";
    }
    std::cout << std::endl;

    std::cout << "  Sample Rate:   " << info.sampleRate << " Hz" << std::endl;
    std::cout << "  Channels:      " << channelModeName(info.channelMode) << std::endl;
    std::cout << "  Frames:        " << info.frameCount << std::endl;

    double durationSec = info.totalDurationMs / 1000.0;
    std::cout << "  Duration:      " << std::fixed << std::setprecision(1)
              << durationSec << " s";
    if (durationSec >= 60.0) {
        int mins = static_cast<int>(durationSec) / 60;
        int secs = static_cast<int>(durationSec) % 60;
        std::cout << " (" << mins << "m " << secs << "s)";
    }
    std::cout << std::endl;

    std::cout << "  Audio Data:    " << info.audioDataSize << " bytes";
    if (fileSize > 0) {
        double pct = (double)info.audioDataSize / fileSize * 100.0;
        std::cout << " (" << std::fixed << std::setprecision(1) << pct << "% of file)";
    }
    std::cout << std::endl;

    std::cout << std::string(50, '=') << std::endl;

    return 0;
}

Mp3AudioInfo scanAudioFrames(const std::string& filepath) {
    Mp3AudioInfo info;

    // Open file binary
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return info;

    std::streamsize fileSize = file.tellg();
    if (fileSize < 4) return info;

    // Read entire file
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        return info;
    }
    file.close();

    // Skip ID3v2 tag if present (first 10 bytes header)
    uint64_t pos = 0;
    if (buffer.size() >= 10 &&
        buffer[0] == 'I' && buffer[1] == 'D' && buffer[2] == '3') {
        // ID3v2 header found, calculate tag size (synchsafe integer)
        uint32_t tagSize = 0;
        tagSize = (tagSize << 7) | (buffer[6] & 0x7F);
        tagSize = (tagSize << 7) | (buffer[7] & 0x7F);
        tagSize = (tagSize << 7) | (buffer[8] & 0x7F);
        tagSize = (tagSize << 7) | (buffer[9] & 0x7F);
        pos = 10 + tagSize;  // Skip header + tag data
    }

    uint64_t dataEnd = buffer.size();

    // Skip ID3v1 tag at end if present
    if (dataEnd >= 128 &&
        buffer[dataEnd - 128] == 'T' &&
        buffer[dataEnd - 127] == 'A' &&
        buffer[dataEnd - 126] == 'G') {
        dataEnd -= 128;
    }

    int frameCount = 0;
    uint32_t totalBitrate = 0;
    int firstMpegVer = -1, firstLayer = -1, firstSampleRate = -1, firstChannel = -1;
    bool consistent = true;

    // Scan frames
    while (pos < dataEnd - 4) {
        // Look for sync word 0xFFE? (first 11 bits)
        if (buffer[pos] == 0xFF && (buffer[pos + 1] & 0xE0) == 0xE0) {
            uint8_t b2 = buffer[pos + 1];
            uint8_t b3 = buffer[pos + 2];
            uint8_t b4 = buffer[pos + 3];

            int mpegVer = (b2 >> 3) & 0x03;
            int layer   = (b2 >> 1) & 0x03;
            int brIdx   = (b3 >> 4) & 0x0F;
            int srIdx   = (b3 >> 2) & 0x03;
            bool padding = (b3 >> 1) & 0x01;
            int channel  = (b4 >> 6) & 0x03;

            // Validate
            if (mpegVer == 1 || layer == 0 || brIdx == 0 || brIdx == 15 || srIdx == 3) {
                pos++;
                continue;
            }

            // Determine table indices
            int verTableIdx = (mpegVer == 3) ? 0 : 1;  // MPEG1=0, MPEG2/2.5=1
            int layerTableIdx;
            switch (layer) {
                case 3: layerTableIdx = 0; break;  // Layer I
                case 2: layerTableIdx = 1; break;  // Layer II
                case 1: layerTableIdx = 2; break;  // Layer III
                default: pos++; continue;
            }

            uint16_t bitrate = bitrateTable[verTableIdx][layerTableIdx][brIdx];
            if (bitrate == 0) { pos++; continue; }

            int verSrIdx;
            switch (mpegVer) {
                case 3: verSrIdx = 0; break;  // MPEG1
                case 2: verSrIdx = 1; break;  // MPEG2
                case 0: verSrIdx = 2; break;  // MPEG2.5
                default: verSrIdx = 3;
            }
            uint32_t sampleRate = sampleRateTable[verSrIdx][srIdx];
            if (sampleRate == 0) { pos++; continue; }

            // First frame info
            if (frameCount == 0) {
                firstMpegVer = mpegVer;
                firstLayer = layer;
                firstSampleRate = srIdx;
                firstChannel = channel;
                info.bitrate = bitrate;
                info.sampleRate = sampleRate;
                info.mpegVersionBits = mpegVer;
                info.layerBits = layer;
                info.sampleRateIndex = srIdx;
                info.channelMode = channel;
                info.hasPadding = padding;
            } else {
                if (bitrate != info.bitrate) {
                    info.hasVBR = true;
                }
            }

            // Calculate frame size
            int paddingSize = padding ? 1 : 0;
            uint16_t frameSize;
            if (layer == 3) {  // Layer I
                frameSize = (12 * bitrate * 1000 / sampleRate + paddingSize) * 4;
            } else {  // Layer II or III
                frameSize = 144 * bitrate * 1000 / sampleRate + paddingSize;
            }

            // Skip frame data
            totalBitrate += bitrate;
            frameCount++;
            pos += frameSize;
        } else {
            pos++;
        }
    }

    if (frameCount == 0) return info;

    info.frameCount = frameCount;
    info.audioDataSize = pos;  // approximate
    info.valid = true;

    // Calculate duration
    int samplesPerFrame;
    switch (firstMpegVer) {
        case 3:  // MPEG1
            switch (firstLayer) {
                case 3: samplesPerFrame = 384; break;   // Layer I
                case 2: samplesPerFrame = 1152; break;  // Layer II
                case 1: samplesPerFrame = 1152; break;  // Layer III
                default: samplesPerFrame = 1152;
            }
            break;
        case 2:  // MPEG2
        case 0:  // MPEG2.5
            switch (firstLayer) {
                case 3: samplesPerFrame = 384; break;   // Layer I
                case 2: samplesPerFrame = 1152; break;  // Layer II
                case 1: samplesPerFrame = 576; break;   // Layer III
                default: samplesPerFrame = 576;
            }
            break;
        default:
            samplesPerFrame = 1152;
    }

    if (info.sampleRate > 0) {
        info.totalDurationMs = (double)frameCount * samplesPerFrame / info.sampleRate * 1000.0;
    }

    // Average bitrate for VBR
    if (info.hasVBR && frameCount > 0) {
        info.bitrate = static_cast<uint16_t>(totalBitrate / frameCount);
    }

    return info;
}
