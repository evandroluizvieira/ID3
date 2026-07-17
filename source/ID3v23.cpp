#include "ID3v23.hpp"

#include <cstring>

namespace{

/**
 * @brief ISO-8859-1 text encoding byte as defined in the ID3v2.3 specification.
 *
 * All text frames store this byte as the first byte of the frame data to indicate
 * that the string content is encoded in ISO-8859-1 (Latin-1).
 */
static const uint8_t ISO_ENCODING_BYTE = 0x00;

}

ID3v23Header::ID3v23Header() :
	ID3v2HeaderBase(){
    major_version = 3;
}

ID3v23Header::~ID3v23Header(){

}

ID3v23ExtendedHeader::ID3v23ExtendedHeader() :
	size(0), flags(0), padding(0), crc(nullptr){

}

ID3v23ExtendedHeader::~ID3v23ExtendedHeader(){
	size = 0;
	flags = 0;
	padding = 0;
	removeCRC();
}

bool ID3v23ExtendedHeader::hasCRC() const{
	return flags & 0x8000;
}

void ID3v23ExtendedHeader::setCRC(uint32_t crcValue){
	if(crc == nullptr){
		crc = new uint32_t;
	}
	flags |= 0x8000;
	*crc = crcValue;
}

uint32_t ID3v23ExtendedHeader::getCRC() const{
	if(hasCRC() == true && crc != nullptr){
		return *crc;
	}
	return 0;
}

void ID3v23ExtendedHeader::removeCRC(){
	flags &= ~0x8000;
	if (crc != nullptr){
		delete crc;
		crc = nullptr;
	}
}

uint32_t ID3v23ExtendedHeader::getSize() const{
	uint32_t sizeFieldLength = 4;
	return size + sizeFieldLength;
}

ID3v23FrameHeader::ID3v23FrameHeader() :
    ID3v2FrameHeaderBase(){
}

ID3v23FrameHeader::~ID3v23FrameHeader(){
}

uint32_t ID3v23FrameHeader::getFrameSize() const{
    uint32_t size = 0;
    size |= static_cast<uint32_t>(this->size[0]) << 24;
    size |= static_cast<uint32_t>(this->size[1]) << 16;
    size |= static_cast<uint32_t>(this->size[2]) << 8;
    size |= this->size[3];
    return size;
}

void ID3v23FrameHeader::setFrameSize(uint32_t size){
    this->size[0] = (size >> 24) & 0xFF;
    this->size[1] = (size >> 16) & 0xFF;
    this->size[2] = (size >> 8) & 0xFF;
    this->size[3] = size & 0xFF;
}

void ID3v23FrameHeader::setEncrypted(bool encrypted){
    if(encrypted)
        flags[1] |= 0x40;
    else
        flags[1] &= ~0x40;
}

bool ID3v23FrameHeader::isEncrypted() const{
    return (flags[1] & 0x40) != 0;
}

void ID3v23FrameHeader::setCompressed(bool compressed){
    if(compressed)
        flags[1] |= 0x80;
    else
        flags[1] &= ~0x80;
}

bool ID3v23FrameHeader::isCompressed() const{
    return (flags[1] & 0x80) != 0;
}

void ID3v23FrameHeader::setReadOnly(bool readOnly){
    if(readOnly)
        flags[0] |= 0x20;
    else
        flags[0] &= ~0x20;
}

bool ID3v23FrameHeader::isReadOnly() const{
    return (flags[0] & 0x20) != 0;
}

void ID3v23FrameHeader::setGroupingIdentity(bool group){
    if(group)
        flags[1] |= 0x20;
    else
        flags[1] &= ~0x20;
}

bool ID3v23FrameHeader::isGroupingIdentity() const{
    return (flags[1] & 0x20) != 0;
}

ID3v23Frame::ID3v23Frame() :
    data(nullptr){
}

ID3v23Frame::~ID3v23Frame(){
    if(data != nullptr){
        delete[] data;
        data = nullptr;
    }
}

ID3v23::ID3v23() :
	extendedHeader(nullptr){
}

ID3v23::~ID3v23(){
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
}

ID3v23Frame* ID3v23::getFrame(const uint8_t identifier[4]) const{
	for(auto frame : frames){
		if(std::memcmp(frame->header.identifier, identifier, 4) == 0){
			return frame;
		}
	}
	return nullptr;
}

bool ID3v23::setFrame(const uint8_t identifier[4], uint32_t size, const uint8_t* data){
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

void ID3v23::addFrame(const uint8_t identifier[4], uint32_t size, const uint8_t* data){
	if(size == 0 || data == nullptr){
		return;
	}
	ID3v23Frame* frame = new ID3v23Frame();
	std::memcpy(frame->header.identifier, identifier, 4);
	frame->header.setFrameSize(size);
	frame->data = new uint8_t[size];
	std::memcpy(frame->data, data, size);
	frames.push_back(frame);
}

void ID3v23::removeFrame(ID3v23Frame* frame){
	for(auto it = frames.begin(); it != frames.end(); ++it){
		if(*it == frame){
			delete frame;
			frames.erase(it);
			break;
		}
	}
}

std::string ID3v23::getTitle() const{
	uint8_t identifier[4] = {'T', 'I', 'T', '2'};
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v23::setTitle(const std::string& title){
	uint8_t identifier[4] = {'T', 'I', 'T', '2'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += title;
	uint32_t size = frameData.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		delete[] frame->data;
		frame->data = new uint8_t[size];
		std::memcpy(frame->data, data, size);
		frame->header.setFrameSize(size);
	}else{
		addFrame(identifier, size, data);
	}
}

std::string ID3v23::getArtist() const{
	uint8_t identifier[4] = {'T', 'P', 'E', '1'};
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v23::setArtist(const std::string& artist){
	uint8_t identifier[4] = {'T', 'P', 'E', '1'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += artist;
	uint32_t size = frameData.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		delete[] frame->data;
		frame->data = new uint8_t[size];
		std::memcpy(frame->data, data, size);
		frame->header.setFrameSize(size);
	}else{
		addFrame(identifier, size, data);
	}
}

std::string ID3v23::getAlbum() const{
	uint8_t identifier[4] = {'T', 'A', 'L', 'B'};
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v23::setAlbum(const std::string& album){
	uint8_t identifier[4] = {'T', 'A', 'L', 'B'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += album;
	uint32_t size = frameData.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		delete[] frame->data;
		frame->data = new uint8_t[size];
		std::memcpy(frame->data, data, size);
		frame->header.setFrameSize(size);
	}else{
		addFrame(identifier, size, data);
	}
}

std::string ID3v23::getYear() const{
	uint8_t identifier[4] = {'T', 'Y', 'E', 'R'};
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v23::setYear(const std::string& year){
	if(year.size() != 4){
		return;
	}
	uint8_t identifier[4] = {'T', 'Y', 'E', 'R'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += year;
	uint32_t size = frameData.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		delete[] frame->data;
		frame->data = new uint8_t[size];
		std::memcpy(frame->data, data, size);
		frame->header.setFrameSize(size);
	}else{
		addFrame(identifier, size, data);
	}
}

std::string ID3v23::getComment() const{
    uint8_t identifier[4] = {'C', 'O', 'M', 'M'};
    ID3v23Frame* frame = getFrame(identifier);
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

void ID3v23::setComment(const std::string& comment){
	uint8_t identifier[4] = {'C', 'O', 'M', 'M'};

	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += "eng"; // language
	frameData += '\0'; // empty short content description
	frameData += comment;

	uint32_t size = frameData.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		delete[] frame->data;
		frame->data = new uint8_t[size];
		std::memcpy(frame->data, data, size);
		frame->header.setFrameSize(size);
	}else{
		addFrame(identifier, size, data);
	}
}

std::string ID3v23::getTrack() const{
	uint8_t identifier[4] = {'T', 'R', 'C', 'K'};
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v23::setTrack(uint8_t track){
	uint8_t identifier[4] = {'T', 'R', 'C', 'K'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += std::to_string(track);
	uint32_t size = frameData.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		delete[] frame->data;
		frame->data = new uint8_t[size];
		std::memcpy(frame->data, data, size);
		frame->header.setFrameSize(size);
	}else{
		addFrame(identifier, size, data);
	}
}

std::string ID3v23::getGenre() const{
	uint8_t identifier[4] = {'T', 'C', 'O', 'N'};
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v23::setGenre(ID3v10::Genre genre){
	uint8_t identifier[4] = {'T', 'C', 'O', 'N'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += '(' + std::to_string(static_cast<int>(genre)) + ')';
	uint32_t size = frameData.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str()));
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		delete[] frame->data;
		frame->data = new uint8_t[size];
		std::memcpy(frame->data, data, size);
		frame->header.setFrameSize(size);
	}else{
		addFrame(identifier, size, data);
	}
}
