#include "ID3v20.hpp"

#include <cstring>

ID3v20Header::ID3v20Header() :
	ID3v2HeaderBase(){
	major_version = 2;
}

ID3v20Header::~ID3v20Header(){

}

bool ID3v20Header::isCompressed() const{
	return (flags & 0x40) != 0;
}

void ID3v20Header::setCompressed(bool useCompression){
	if(useCompression){
		flags |= 0x40;
	}else{
		flags &= ~0x40;
	}
}

ID3v20FrameHeader::ID3v20FrameHeader() :
	identifier{0, 0, 0}, size{0, 0, 0}{

}

ID3v20FrameHeader::~ID3v20FrameHeader(){
	identifier[0] = 0;
	identifier[1] = 0;
	identifier[2] = 0;
	size[0] = 0;
	size[1] = 0;
	size[2] = 0;
}

uint32_t ID3v20FrameHeader::getFrameSize() const{
	// Calculate size from the 3-byte size field
	uint32_t size = 0;
	size |= static_cast<uint32_t>(this->size[0]) << 16;
	size |= static_cast<uint32_t>(this->size[1]) << 8;
	size |= static_cast<uint32_t>(this->size[2]);
	return size;
}

void ID3v20FrameHeader::setFrameSize(uint32_t size){
	// Ensure size fits within the 3-byte limit
	if(size > 0xFFFFFF){
		size = 0xFFFFFF;
	}

	this->size[0] = static_cast<uint8_t>((size >> 16) & 0xFF);
	this->size[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
	this->size[2] = static_cast<uint8_t>(size & 0xFF);
}

ID3v20Frame::ID3v20Frame() :
	data(nullptr){
	header = {};
}

ID3v20Frame::~ID3v20Frame(){
	header = {};

	if(data != nullptr){
		delete data;
		data = nullptr;
	}
}

ID3v20::ID3v20(){
	header = {};
}

ID3v20::~ID3v20(){
	for(auto& frame : frames){
		delete frame;
		frame = nullptr;
	}
	frames.clear();

	header = {};
}

ID3v20Frame* ID3v20::getFrame(uint8_t identifier[3]) const{
	for(auto frame : frames){
		if(std::memcmp(frame->header.identifier, identifier, 3) == 0){
			return frame;
		}
	}
	return nullptr;
}

void ID3v20::setFrame(uint8_t identifier[3], uint32_t size, uint8_t* data){
	ID3v20Frame* frame = getFrame(identifier);

	if(frame == nullptr){
		frame = new ID3v20Frame();
	}

	if(frame->data != nullptr){
		delete[] frame->data;
	}

	frame->data = new uint8_t[size];
	std::memcpy(frame->data, data, size);
	std::memcpy(frame->header.identifier, identifier, 3);
	frame->header.setFrameSize(size);

	frames.push_back(frame);
}

void ID3v20::removeFrame(uint8_t identifier[3]){
	for (auto it = frames.begin(); it != frames.end(); ){
		ID3v20Frame* frame = *it;

		if(std::memcmp(frame->header.identifier, identifier, 3) == 0){
			delete frame;
			it = frames.erase(it);
			return;
		}else{
			++it;
		}
	}
}

std::string ID3v20::getTitle() const{
	uint8_t identifier[3] = {'T', 'T', '2'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v20::setTitle(const std::string& title){
	uint8_t identifier[3] = {'T', 'T', '2'};
	setFrame(identifier, title.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(title.c_str())));
}

std::string ID3v20::getArtist() const{
	uint8_t identifier[3] = {'T', 'P', '1'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v20::setArtist(const std::string& artist){
	uint8_t identifier[3] = {'T', 'P', '1'};
	setFrame(identifier, artist.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(artist.c_str())));
}

std::string ID3v20::getAlbum() const{
	uint8_t identifier[3] = {'T', 'A', 'L'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v20::setAlbum(const std::string& album){
	uint8_t identifier[3] = {'T', 'A', 'L'};
	setFrame(identifier, album.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(album.c_str())));
}

std::string ID3v20::getYear() const{
	uint8_t identifier[3] = {'T', 'Y', 'E'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		return std::string(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
	}
	return "";
}

void ID3v20::setYear(const std::string& year){
	uint8_t identifier[3] = {'T', 'Y', 'E'};
	setFrame(identifier, year.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(year.c_str())));
}

std::string ID3v20::getComment() const{
	uint8_t identifier[3] = {'C', 'O', 'M'};
	ID3v20Frame* frame = getFrame(identifier);

	if(frame != nullptr){
		// Convert frame data to a string
		std::string frameData(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());

		//see more languages in: https://www.loc.gov/standards/iso639-2/php/code_list.php
		std::string language = "eng";

		// Find language code position
		size_t languagePosition = frameData.find(language);
		if(languagePosition == std::string::npos){
			return "";
		}

		// Find the first comment position
		size_t commentPos = frameData.find('\0', languagePosition + language.size() + 1);
		if (commentPos == std::string::npos) {
			return "";
		}

		// Extract the comment text
		return frameData.substr(commentPos + 1);
	}

	return "";
}

void ID3v20::setComment(const std::string& comment){
	uint8_t identifier[3] = {'C', 'O', 'M'};

	//see more languages in: https://www.loc.gov/standards/iso639-2/php/code_list.php
	std::string language = "eng";

	// Construct the frame data including language and comment
	std::string frameData;
	frameData += language;
	frameData += '\0'; // Null separator
	frameData += comment;

	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}


uint8_t ID3v20::getTrack() const{
	uint8_t identifier[3] = {'T', 'R', 'K'};
	ID3v20Frame* frame = getFrame(identifier);

	if(frame != nullptr){
		std::string trackString(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
		size_t slashPosition = trackString.find('/');
		if(slashPosition != std::string::npos){
			trackString = trackString.substr(0, slashPosition);
		}
		return std::stoul(trackString);
	}

	return 0;
}

void ID3v20::setTrack(uint8_t track){
	uint8_t identifier[3] = {'T', 'R', 'K'};
	std::string trackString = std::to_string(track);
	setFrame(identifier, trackString.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(trackString.c_str())));
}

ID3v10::Genre ID3v20::getGenre() const{
	uint8_t identifier[3] = {'T', 'C', 'O'};
	ID3v20Frame* frame = getFrame(identifier);

	if(frame != nullptr){
		std::string genreString(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());
		if(genreString.front() == '(' && genreString.back() == ')'){
			genreString = genreString.substr(1, genreString.size() - 2);
		}

		int genreValue = std::stoi(genreString);
		if(genreValue >= 0 && genreValue <= ID3v10::HardRock){
			return static_cast<ID3v10::Genre>(genreValue);
		}
	}

	return ID3v10::Genre::Other;
}

void ID3v20::setGenre(ID3v10::Genre genre){
	uint8_t identifier[3] = {'T', 'C', 'O'};

	std::string genreString = "(" + std::to_string(static_cast<int>(genre)) + ")";
	setFrame(identifier, genreString.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(genreString.c_str())));
}
