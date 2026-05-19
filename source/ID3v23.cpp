#include "ID3v23.hpp"

#include <cstring>

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

#include "ID3v23FrameHeader.hpp"

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
        flags[0] |= 0x01;
    else
        flags[0] &= ~0x01;
}

bool ID3v23FrameHeader::isEncrypted() const{
    return (flags[0] & 0x01) != 0;
}

void ID3v23FrameHeader::setCompressed(bool compressed){
    if(compressed)
        flags[0] |= 0x08;
    else
        flags[0] &= ~0x08;
}

bool ID3v23FrameHeader::isCompressed() const{
    return (flags[0] & 0x08) != 0;
}

void ID3v23FrameHeader::setFileAlterPreservation(bool preserve){
    if(preserve)
        flags[0] |= 0x20;
    else
        flags[0] &= ~0x20;
}

bool ID3v23FrameHeader::getFileAlterPreservation() const{
    return (flags[0] & 0x20) != 0;
}

void ID3v23FrameHeader::setTagAlterPreservation(bool preserve){
    if(preserve)
        flags[0] |= 0x40;
    else
        flags[0] &= ~0x40;
}

bool ID3v23FrameHeader::getTagAlterPreservation() const{
    return (flags[0] & 0x40) != 0;
}

void ID3v23FrameHeader::setReadOnly(bool readOnly){
    if(readOnly)
        flags[0] |= 0x80;
    else
        flags[0] &= ~0x80;
}

bool ID3v23FrameHeader::isReadOnly() const{
    return (flags[0] & 0x80) != 0;
}

void ID3v23FrameHeader::setGroupingIdentity(bool group){
    if(group)
        flags[1] |= 0x80;
    else
        flags[1] &= ~0x80;
}

bool ID3v23FrameHeader::isGroupingIdentity() const{
    return (flags[1] & 0x80) != 0;
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

ID3v23Frame* ID3v23::getFrame(uint8_t identifier[4]) const{
	for(auto frame : frames){
		if(std::memcmp(frame->header.identifier, identifier, 4) == 0){
			return frame;
		}
	}
	return nullptr;
}

bool ID3v23::setFrame(uint8_t identifier[4], uint32_t size, uint8_t* data){
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

void ID3v23::addFrame(uint8_t identifier[4], uint32_t size, uint8_t* data){
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
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v23::setTitle(const std::string& title){
	uint8_t identifier[4] = {'T', 'I', 'T', '2'};
	uint32_t size = title.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(title.c_str()));
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
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v23::setArtist(const std::string& title){
	uint8_t identifier[4] = {'T', 'P', 'E', '1'};
	uint32_t size = title.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(title.c_str()));
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
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v23::setAlbum(const std::string& title){
	uint8_t identifier[4] = {'T', 'A', 'L', 'B'};
	uint32_t size = title.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(title.c_str()));
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
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v23::setYear(const std::string& year){
	if(year.size() == 4){
		uint8_t identifier[4] = {'T', 'Y', 'E', 'R'};
		uint32_t size = year.size();
		uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(year.c_str()));
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
}

std::string ID3v23::getComment() const{
	uint8_t identifier[4] = {'C', 'O', 'M', 'M'};
	ID3v23Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

std::string ID3v23::getComment() const{
    uint8_t identifier[4] = {'C', 'O', 'M', 'M'};
    ID3v23Frame* frame = getFrame(identifier);
    if(!frame || !frame->data){
		return "";
	}

    uint32_t size = frame->header.getFrameSize();
    if(size < 5){
		return "";
	}
    
	uint8_t encodingISO_8859_1 = 0x00;
	uint8_t encodingUtf8 = 0x03;

	uint8_t encoding = frame->data[0];
    //std::string language = std::string(reinterpret_cast<const char*>(frame->data + 1), 3);

    const char* descriptionStart = reinterpret_cast<const char*>(frame->data + 4);
    uint32_t descriptionLength = strnlen(descriptionStart, size - 4);
    
	const char* textStart = descriptionStart + descriptionLength + 1;
    uint32_t textLength = size - (4 + descriptionLength + 1);

    if(encoding == encodingISO_8859_1 || encoding == encodingUtf8){
        return std::string(textStart, strnlen(textStart, textLength));
    }
    
    return "";
}

void ID3v23::setComment(const std::string& comment){
	uint8_t identifier[4] = {'C', 'O', 'M', 'M'};
	uint8_t encodingISO_8859_1 = 0x00;
	//uint8_t encodingUtf8 = 0x03;
	std::string language = "eng";

	std::string frameData;
	frameData += static_cast<char>(encodingISO_8859_1);
	frameData += language;
	frameData += '\0';
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
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v23::setTrack(uint8_t track){
	uint8_t identifier[4] = {'T', 'R', 'C', 'K'};
	std::string trackString = std::to_string(track);
	uint32_t size = trackString.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(trackString.c_str()));
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
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v23::setGenre(ID3v10::Genre genre){
	uint8_t identifier[4] = {'T', 'C', 'O', 'N'};
	std::string genreString = '(' + std::to_string(genre) + ')';
	uint32_t size = genreString.size();
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(genreString.c_str()));
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
