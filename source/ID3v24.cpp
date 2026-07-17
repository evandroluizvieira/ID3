#include "ID3v24.hpp"

#include <cstring>

namespace{

/**
 * @brief ISO-8859-1 text encoding byte as defined in the ID3v2.4 specification.
 *
 * All text frames store this byte as the first byte of the frame data to indicate
 * that the string content is encoded in ISO-8859-1 (Latin-1).
 */
static const uint8_t ISO_ENCODING_BYTE = 0x00;

}

ID3v24Header::ID3v24Header() :
    ID3v2HeaderBase(){
    major_version = 4;
}

ID3v24Header::~ID3v24Header(){

}

void ID3v24Header::setFooter(bool hasFooter){
    if(hasFooter){
        flags |= 0x10;
    }else{
        flags &= ~0x10;
    }
}

bool ID3v24Header::hasFooter() const {
    return (flags & 0x10) != 0;
}

ID3v24ExtendedHeader::ID3v24ExtendedHeader() :
	size(2), flagBytes(1), flags(0), crc(nullptr){
}

ID3v24ExtendedHeader::~ID3v24ExtendedHeader(){
	size = 0;
	flagBytes = 0;
	flags = 0;
	removeCRC();
}

bool ID3v24ExtendedHeader::hasCRC() const{
	return (flags & 0x20) != 0;
}

void ID3v24ExtendedHeader::setCRC(uint32_t crcValue){
	if(crc == nullptr){
		crc = new uint32_t;
	}

	flags |= 0x20;
	*crc = crcValue;
	
    // Recalculate size: base 2 (flagBytes+flags) + update length (1) + CRC length (5)
	size = 2;
	if(isUpdate()){
        size += 1;
    }
	size += 5;
}

uint32_t ID3v24ExtendedHeader::getCRC() const{
	if(hasCRC() && crc != nullptr){
		return *crc;
	}
	return 0;
}

void ID3v24ExtendedHeader::removeCRC(){
	flags &= ~0x20;
	if(crc != nullptr){
		delete crc;
		crc = nullptr;
	}
	size = 2;
	if(isUpdate()) size += 1;
}

bool ID3v24ExtendedHeader::isUpdate() const{
	return (flags & 0x40) != 0;
}

void ID3v24ExtendedHeader::setUpdate(bool update){
	if(update){
		flags |= 0x40;
		size = 2 + 1;
		if(hasCRC()) size += 5;
	}else{
		flags &= ~0x40;
		size = 2;
		if(hasCRC()) size += 5;
	}
}

uint32_t ID3v24ExtendedHeader::getSize() const{
	uint32_t sizeFieldLength = 4;
	return size + sizeFieldLength;
}

ID3v24FrameHeader::ID3v24FrameHeader() :
    ID3v2FrameHeaderBase(){
}

ID3v24FrameHeader::~ID3v24FrameHeader(){
}

uint32_t ID3v24FrameHeader::getFrameSize() const{
    uint32_t size = 0;
    size |= static_cast<uint32_t>(this->size[0]) << 21;
    size |= static_cast<uint32_t>(this->size[1]) << 14;
    size |= static_cast<uint32_t>(this->size[2]) << 7;
    size |= this->size[3];
    return size;
}

void ID3v24FrameHeader::setFrameSize(uint32_t size){
    this->size[0] = (size >> 21) & 0x7F;
    this->size[1] = (size >> 14) & 0x7F;
    this->size[2] = (size >> 7) & 0x7F;
    this->size[3] = size & 0x7F;
}

void ID3v24FrameHeader::setDataLengthIndicator(bool hasIndicator){
    if(hasIndicator)
        flags[1] |= 0x01;
    else
        flags[1] &= ~0x01;
}

bool ID3v24FrameHeader::hasDataLengthIndicator() const{
    return (flags[1] & 0x01) != 0;
}

void ID3v24FrameHeader::setUnsynchronised(bool unsync){
    if(unsync)
        flags[1] |= 0x02;
    else
        flags[1] &= ~0x02;
}

bool ID3v24FrameHeader::isUnsynchronised() const{
    return (flags[1] & 0x02) != 0;
}

void ID3v24FrameHeader::setEncrypted(bool encrypted){
    if(encrypted)
        flags[1] |= 0x04;
    else
        flags[1] &= ~0x04;
}

bool ID3v24FrameHeader::isEncrypted() const{
    return (flags[1] & 0x04) != 0;
}

void ID3v24FrameHeader::setCompressed(bool compressed){
    if(compressed)
        flags[1] |= 0x08;
    else
        flags[1] &= ~0x08;
}

bool ID3v24FrameHeader::isCompressed() const{
    return (flags[1] & 0x08) != 0;
}

void ID3v24FrameHeader::setReadOnly(bool readOnly){
    if(readOnly)
        flags[0] |= 0x10;
    else
        flags[0] &= ~0x10;
}

bool ID3v24FrameHeader::isReadOnly() const{
    return (flags[0] & 0x10) != 0;
}

void ID3v24FrameHeader::setGroupingIdentity(bool group){
    if(group)
        flags[1] |= 0x40;
    else
        flags[1] &= ~0x40;
}

bool ID3v24FrameHeader::isGroupingIdentity() const{
    return (flags[1] & 0x40) != 0;
}

ID3v24Frame::ID3v24Frame() :
    data(nullptr){
}

ID3v24Frame::~ID3v24Frame(){
    if(data != nullptr){
        delete[] data;
        data = nullptr;
    }
}

ID3v24Footer::ID3v24Footer() :
    identifier{'3', 'D', 'I'}, major_version(4), revision(0), flags(0), size{0, 0, 0, 0}{

}

ID3v24Footer::~ID3v24Footer(){
}

void ID3v24Footer::setFlags(uint8_t flagsValue){
    flags = flagsValue;
}

uint8_t ID3v24Footer::getFlags() const{
    return flags;
}

void ID3v24Footer::setSize(uint32_t tagSize){
    if(tagSize > 0x0FFFFFFF) tagSize = 0x0FFFFFFF;
    size[0] = (tagSize >> 21) & 0x7F;
    size[1] = (tagSize >> 14) & 0x7F;
    size[2] = (tagSize >> 7) & 0x7F;
    size[3] = tagSize & 0x7F;
}

uint32_t ID3v24Footer::getSize() const{
    uint32_t tagSize = 0;
    tagSize |= static_cast<uint32_t>(size[0]) << 21;
    tagSize |= static_cast<uint32_t>(size[1]) << 14;
    tagSize |= static_cast<uint32_t>(size[2]) << 7;
    tagSize |= size[3];
    return tagSize;
}

ID3v24::ID3v24() :
    extendedHeader(nullptr), footer(nullptr){
}

ID3v24::~ID3v24(){
    for(auto& frame : frames){
        if(frame){
            delete frame;
        }
    }
    frames.clear();

    if(extendedHeader != nullptr){
        delete extendedHeader;
        extendedHeader = nullptr;
    }

    if(footer != nullptr){
        delete footer;
        footer = nullptr;
    }
}

ID3v24Frame* ID3v24::getFrame(const uint8_t identifier[4]) const{
    for(auto frame : frames){
        if(std::memcmp(frame->header.identifier, identifier, 4) == 0){
            return frame;
        }
    }
    return nullptr;
}

bool ID3v24::setFrame(const uint8_t identifier[4], uint32_t size, const uint8_t* data){
    if(size == 0 || data == nullptr){
        return false;
    }

    for(auto& frame : frames){
        if(std::memcmp(frame->header.identifier, identifier, 4) == 0){
            delete[] frame->data;
            frame->data = new uint8_t[size];
            std::memcpy(frame->data, data, size);
            frame->header.setFrameSize(size);
            return true;
        }
    }
    return false;
}

void ID3v24::addFrame(const uint8_t identifier[4], uint32_t size, const uint8_t* data){
    if(size == 0 || data == nullptr){
        return;
    }
    ID3v24Frame* frame = new ID3v24Frame();
    std::memcpy(frame->header.identifier, identifier, 4);
    frame->header.setFrameSize(size);
    frame->data = new uint8_t[size];
    std::memcpy(frame->data, data, size);
    frames.push_back(frame);
}

void ID3v24::removeFrame(ID3v24Frame* frame){
    for(auto it = frames.begin(); it != frames.end(); ++it){
        if(*it == frame){
            delete frame;
            frames.erase(it);
            break;
        }
    }
}

std::string ID3v24::getTitle() const{
    uint8_t identifier[4] = {'T', 'I', 'T', '2'};
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        const uint32_t frameSize = frame->header.getFrameSize();
        if(frameSize <= 1){
            return "";
        }
        return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
    }
    return "";
}

void ID3v24::setTitle(const std::string& title){
    uint8_t identifier[4] = {'T', 'I', 'T', '2'};
    std::string frameData;
    frameData += static_cast<char>(ISO_ENCODING_BYTE);
    frameData += title;
    uint32_t size = frameData.size();
    uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        delete[] frame->data;
        frame->data = new uint8_t[size];
        std::memcpy(frame->data, data, size);
        frame->header.setFrameSize(size);
    }else{
        addFrame(identifier, size, data);
    }
}

std::string ID3v24::getArtist() const{
    uint8_t identifier[4] = {'T', 'P', 'E', '1'};
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        const uint32_t frameSize = frame->header.getFrameSize();
        if(frameSize <= 1){
            return "";
        }
        return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
    }
    return "";
}

void ID3v24::setArtist(const std::string& artist){
    uint8_t identifier[4] = {'T', 'P', 'E', '1'};
    std::string frameData;
    frameData += static_cast<char>(ISO_ENCODING_BYTE);
    frameData += artist;
    uint32_t size = frameData.size();
    uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        delete[] frame->data;
        frame->data = new uint8_t[size];
        std::memcpy(frame->data, data, size);
        frame->header.setFrameSize(size);
    }else{
        addFrame(identifier, size, data);
    }
}

std::string ID3v24::getAlbum() const{
    uint8_t identifier[4] = {'T', 'A', 'L', 'B'};
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        const uint32_t frameSize = frame->header.getFrameSize();
        if(frameSize <= 1){
            return "";
        }
        return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
    }
    return "";
}

void ID3v24::setAlbum(const std::string& album){
    uint8_t identifier[4] = {'T', 'A', 'L', 'B'};
    std::string frameData;
    frameData += static_cast<char>(ISO_ENCODING_BYTE);
    frameData += album;
    uint32_t size = frameData.size();
    uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        delete[] frame->data;
        frame->data = new uint8_t[size];
        std::memcpy(frame->data, data, size);
        frame->header.setFrameSize(size);
    }else{
        addFrame(identifier, size, data);
    }
}

std::string ID3v24::getYear() const{
    uint8_t identifier[4] = {'T', 'D', 'R', 'C'};
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        const uint32_t frameSize = frame->header.getFrameSize();
        if(frameSize <= 1){
            return "";
        }
        std::string value(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
        if(value.size() >= 4){
            return value.substr(0, 4);
        }
    }
    return "";
}

void ID3v24::setYear(const std::string& year){
    if(year.size() != 4){
        return;
    }
    uint8_t identifier[4] = {'T', 'D', 'R', 'C'};
    std::string frameData;
    frameData += static_cast<char>(ISO_ENCODING_BYTE);
    frameData += year;
    uint32_t size = frameData.size();
    uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        delete[] frame->data;
        frame->data = new uint8_t[size];
        std::memcpy(frame->data, data, size);
        frame->header.setFrameSize(size);
    }else{
        addFrame(identifier, size, data);
    }
}

std::string ID3v24::getComment() const{
    uint8_t identifier[4] = {'C', 'O', 'M', 'M'};
    ID3v24Frame* frame = getFrame(identifier);
    if(!frame || !frame->data){
        return "";
    }

    const uint32_t frameSize = frame->header.getFrameSize();
    // Minimum: encoding(1) + language(3) + description null terminator(1) = 5 bytes
    if(frameSize < 5){
        return "";
    }

    const char* rawData = reinterpret_cast<const char*>(frame->data);
    // Skip encoding byte (rawData[0]) and language bytes (rawData[1..3]).
    // Walk from rawData[4] to find the null terminator of the short content description.
    size_t descriptionEnd = 4;
    while(descriptionEnd < frameSize && rawData[descriptionEnd] != '\0'){
        ++descriptionEnd;
    }
    if(descriptionEnd >= frameSize){
        return "";
    }

    return std::string(rawData + descriptionEnd + 1, frameSize - descriptionEnd - 1);
}

void ID3v24::setComment(const std::string& comment){
    uint8_t identifier[4] = {'C', 'O', 'M', 'M'};

    std::string frameData;
    frameData += static_cast<char>(ISO_ENCODING_BYTE);
    frameData += "eng"; // language
    frameData += '\0'; // empty short content description
    frameData += comment;

    uint32_t size = frameData.size();
    uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        delete[] frame->data;
        frame->data = new uint8_t[size];
        std::memcpy(frame->data, data, size);
        frame->header.setFrameSize(size);
    }else{
        addFrame(identifier, size, data);
    }
}

std::string ID3v24::getTrack() const{
    uint8_t identifier[4] = {'T', 'R', 'C', 'K'};
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        const uint32_t frameSize = frame->header.getFrameSize();
        if(frameSize <= 1){
            return "";
        }
        return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
    }
    return "";
}

void ID3v24::setTrack(uint8_t track){
    uint8_t identifier[4] = {'T', 'R', 'C', 'K'};
    std::string frameData;
    frameData += static_cast<char>(ISO_ENCODING_BYTE);
    frameData += std::to_string(track);
    uint32_t size = frameData.size();
    uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        delete[] frame->data;
        frame->data = new uint8_t[size];
        std::memcpy(frame->data, data, size);
        frame->header.setFrameSize(size);
    }else{
        addFrame(identifier, size, data);
    }
}

std::string ID3v24::getGenre() const{
    uint8_t identifier[4] = {'T', 'C', 'O', 'N'};
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        const uint32_t frameSize = frame->header.getFrameSize();
        if(frameSize <= 1){
            return "";
        }
        return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
    }
    return "";
}

void ID3v24::setGenre(uint8_t genre){
    uint8_t identifier[4] = {'T', 'C', 'O', 'N'};
    std::string frameData;
    frameData += static_cast<char>(ISO_ENCODING_BYTE);
    frameData += '(' + std::to_string(genre) + ')';
    uint32_t size = frameData.size();
    uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
    ID3v24Frame* frame = getFrame(identifier);
    if(frame != nullptr){
        delete[] frame->data;
        frame->data = new uint8_t[size];
        std::memcpy(frame->data, data, size);
        frame->header.setFrameSize(size);
    }else{
        addFrame(identifier, size, data);
    }
}