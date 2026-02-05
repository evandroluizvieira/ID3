#include "ID3v23.hpp"

#include <cstring>

ID3v23Header::ID3v23Header() :
	ID3v2HeaderBase(){
    major_version = 3;
}

ID3v23Header::~ID3v23Header(){

}

void ID3v23Header::setExtendedHeader(bool hasExtendedHeader){
	if(hasExtendedHeader){
		flags |= 0x40;
	}else{
		flags &= ~0x40;
	}
}

bool ID3v23Header::hasExtendedHeader() const{
	return (flags & 0x40) != 0;
}

void ID3v23Header::setExperimental(bool isExperimental){
	if(isExperimental){
		flags |= 0x20;
	}else{
		flags &= ~0x20;
	}
}

bool ID3v23Header::isExperimental() const{
	return (flags & 0x20) != 0;
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
	if(hasCRC() == true){
		uint32_t sizeOfCRC = 4;
		return size + sizeOfCRC;
	}

	return size;
}

ID3v23FrameHeader::ID3v23FrameHeader() :
	identifier{0, 0, 0, 0}, size{0, 0, 0, 0}, flags{0, 0}{

}

ID3v23FrameHeader::~ID3v23FrameHeader(){
	identifier[0] = 0;
	identifier[1] = 0;
	identifier[2] = 0;
	identifier[3] = 0;
	size[0] = 0;
	size[1] = 0;
	size[2] = 0;
	size[3] = 0;
	flags[0] = 0;
	flags[1] = 0;
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

void ID3v23FrameHeader::setTagAlterPreservation(bool preserve){
	if(preserve == true){
		flags[0] |= 0x40;
	} else {
		flags[0] &= ~0x40;
	}
}

bool ID3v23FrameHeader::getTagAlterPreservation() const{
	return (flags[0] & 0x40) != 0;
}

void ID3v23FrameHeader::setFileAlterPreservation(bool preserve){
	if(preserve == true){
		flags[0] |= 0x20;
	} else {
		flags[0] &= ~0x20;
	}
}

bool ID3v23FrameHeader::getFileAlterPreservation() const{
	return (flags[0] & 0x20) != 0;
}

void ID3v23FrameHeader::setReadOnly(bool readOnly){
	if(readOnly == true){
		flags[0] |= 0x80;
	}else{
		flags[0] &= ~0x80;
	}
}

bool ID3v23FrameHeader::isReadOnly() const{
	return (flags[0] & 0x80) != 0;
}

void ID3v23FrameHeader::setCompressed(bool compressed){
	if(compressed == true){
		flags[0] |= 0x08;
	}else{
		flags[0] &= ~0x08;
	}
}

bool ID3v23FrameHeader::isCompressed() const{
	return (flags[0] & 0x08) != 0;
}

void ID3v23FrameHeader::setEncrypted(bool encrypted){
	if(encrypted == true){
		flags[0] |= 0x01;
	}else{
		flags[0] &= ~0x01;
	}
}

bool ID3v23FrameHeader::isEncrypted() const{
	return (flags[0] & 0x01) != 0;
}

void ID3v23FrameHeader::setGroupingIdentity(bool group){
	if(group == true){
		flags[1] |= 0x80;
	}else{
		flags[1] &= ~0x80;
	}
}

bool ID3v23FrameHeader::isGroupingIdentity() const{
	return (flags[1] & 0x80) != 0;
}

ID3v23Frame::ID3v23Frame() :
	data(nullptr){
	header = {};
}

ID3v23Frame::~ID3v23Frame(){
	header = {};

	if(data != nullptr){
		delete[] data;
		data = nullptr;
	}
}

ID3v23::ID3v23() :
	extendedHeader(nullptr){
	header = {};
}

ID3v23::~ID3v23(){
	for(auto& frame : frames){
		delete frame;
		frame = nullptr;
	}
	frames.clear();

	if(extendedHeader != nullptr){
		delete extendedHeader;
		extendedHeader = nullptr;
	}

	header = {};

	extendedHeader = {};
}

#include <iostream>
using std::cout;
using std::endl;

void ID3v23::print(){
	cout << "header.getTagSize()        " << header.getTagSize() << endl;
	cout << "header.isUnsynchronized()  " << header.isUnsynchronized() << endl;
	cout << "header.isExperimental()    " << header.isExperimental() << endl;
	cout << "header.hasExtendedHeader() " << header.hasExtendedHeader() << endl;
	if(header.hasExtendedHeader()){
		cout << "header.hasExtendedHeader() " << extendedHeader->hasCRC() << endl;
		cout << "header.hasExtendedHeader() " << extendedHeader->getCRC() << endl;
		cout << "header.hasExtendedHeader() " << extendedHeader->getSize() << endl;
	}

	int i = 0;
	for(auto frame : frames){
		cout << "frame " << i << endl;
		i++;
		cout << "'" << frame->header.identifier[0] << "', ";
		cout << "'" << frame->header.identifier[1] << "', ";
		cout << "'" << frame->header.identifier[2] << "', ";
		cout << "'" << frame->header.identifier[3] << "'" << endl;

		cout << "header.getFrameSize()             " << frame->header.getFrameSize() << endl;
		cout << "header.getTagAlterPreservation()  " << frame->header.getTagAlterPreservation() << endl;
		cout << "header.getFileAlterPreservation() " << frame->header.getFileAlterPreservation() << endl;
		cout << "header.isReadOnly()               " << frame->header.isReadOnly() << endl;
		cout << "header.isCompressed()             " << frame->header.isCompressed() << endl;
		cout << "header.isEncrypted()              " << frame->header.isEncrypted() << endl;
		cout << "header.isGroupingIdentity()       " << frame->header.isGroupingIdentity() << endl;

		//cout << "frame->data " << std::string((char*)frame->data, frame->header.getFrameSize()) << endl;
		cout << endl;
	}
}

ID3v23Frame* ID3v23::getFrame(uint8_t identifier[4]) const {
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
	setFrame(identifier, title.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(title.c_str())));
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
	setFrame(identifier, title.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(title.c_str())));
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
	setFrame(identifier, title.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(title.c_str())));
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
		setFrame(identifier, year.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(year.c_str())));
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

void ID3v23::setComment(const std::string& comment){
	uint8_t identifier[4] = {'C', 'O', 'M', 'M'};

	// Set text encoding to ISO-8859-1 (0x00) or UTF-16 (0x01)
	uint8_t encoding = 0x00;
	std::string language = "eng"; // Default to English language code

	// Construct the frame data
	std::string frameData;

	// Add encoding byte
	frameData += static_cast<char>(encoding);

	// Add 3-byte language code
	frameData += language;

	// Short content description (empty)
	frameData += '\0';

	// Add the actual comment
	frameData += comment;

	// Set the frame
	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
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

	// Convert track number to string
	std::string trackStr = std::to_string(track);

	// Set the frame with the track number string
	setFrame(identifier, trackStr.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(trackStr.c_str())));
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

	// Construct the genre string
	std::string genreString = "(" + std::to_string(static_cast<int>(genre)) + ")";

	// Set the frame with the genre string
	setFrame(identifier, genreString.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(genreString.c_str())));
}
