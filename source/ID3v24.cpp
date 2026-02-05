#include "ID3v24.hpp"

#include <cstring>
#include <cstdint>

// --- ID3v24Footer ---
ID3v24Footer::ID3v24Footer() {
    id[0] = '3'; id[1] = 'D'; id[2] = 'I';
    version_major = 0x04;
    version_minor = 0x00;
    flags = 0;
    std::memset(size, 0, 4);
}

ID3v24Footer::~ID3v24Footer() {
    clear();
}

void ID3v24Footer::setFooter(uint8_t flags_, uint8_t size_[4]) {
    flags = flags_;
    std::memcpy(size, size_, 4);
}

void ID3v24Footer::clear() {
    id[0] = '3'; id[1] = 'D'; id[2] = 'I';
    version_major = 0x04;
    version_minor = 0x00;
    flags = 0;
    std::memset(size, 0, 4);
}

void ID3v24Footer::toBytes(uint8_t out[10]) const {
    out[0] = id[0]; out[1] = id[1]; out[2] = id[2];
    out[3] = version_major;
    out[4] = version_minor;
    out[5] = flags;
    std::memcpy(&out[6], size, 4);
}

void ID3v24Footer::fromBytes(const uint8_t in[10]) {
    id[0] = in[0]; id[1] = in[1]; id[2] = in[2];
    version_major = in[3];
    version_minor = in[4];
    flags = in[5];
    std::memcpy(size, &in[6], 4);
}

bool ID3v24Footer::isFooter(const uint8_t in[10]) {
    return in[0] == '3' && in[1] == 'D' && in[2] == 'I' && in[3] == 0x04;
}

// --- ID3v24Header ---
ID3v24Header::ID3v24Header() {
    major_version = 4;
}

ID3v24Header::~ID3v24Header() {
}

void ID3v24Header::setFooterPresent(bool hasFooter) {
    if (hasFooter) {
        flags |= 0x10;  // Set bit 4 for footer present flag
    } else {
        flags &= ~0x10; // Clear bit 4
    }
}

bool ID3v24Header::hasFooter() const {
    return (flags & 0x10) != 0;
}

// --- ID3v24 ---
ID3v24::ID3v24() {
}

ID3v24::~ID3v24() {
}

void ID3v24::writeFooter(uint8_t* out) const {
    footer.toBytes(out);
}

void ID3v24::readFooter(const uint8_t* in) {
    footer.fromBytes(in);
}

std::string ID3v24::getYear() const {
    uint8_t identifier[4] = {'T', 'D', 'R', 'C'};
    ID3v23Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
    }
    return "";
}

void ID3v24::setYear(const std::string& year) {
    if(!year.empty()){
        uint8_t identifier[4] = {'T', 'D', 'R', 'C'};
        setFrame(identifier, year.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(year.c_str())));
    }
}
