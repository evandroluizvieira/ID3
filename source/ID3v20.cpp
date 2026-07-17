#include "ID3v20.hpp"

#include <cstring>

namespace{

/**
 * @brief ISO-8859-1 text encoding byte as defined in the ID3v2.0 specification.
 *
 * All text frames store this byte as the first byte of the frame data to indicate
 * that the string content is encoded in ISO-8859-1 (Latin-1).
 */
static const uint8_t ISO_ENCODING_BYTE = 0x00;

}

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
	uint32_t size = 0;
	size |= static_cast<uint32_t>(this->size[0]) << 16;
	size |= static_cast<uint32_t>(this->size[1]) << 8;
	size |= static_cast<uint32_t>(this->size[2]);
	return size;
}

void ID3v20FrameHeader::setFrameSize(uint32_t size){
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
		delete[] data;
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

ID3v20Frame* ID3v20::getFrame(const uint8_t identifier[3]) const{
	for(auto frame : frames){
		if(frame != nullptr && std::memcmp(frame->header.identifier, identifier, 3) == 0){
			return frame;
		}
	}
	return nullptr;
}

void ID3v20::setFrame(const uint8_t identifier[3], uint32_t size, const uint8_t* data){
	if(size == 0 || data == nullptr){
		return;
	}

	ID3v20Frame* frame = getFrame(identifier);

	if(frame == nullptr){
		frame = new ID3v20Frame();
		std::memcpy(frame->header.identifier, identifier, 3);
		frames.push_back(frame);
	}else{
		delete[] frame->data;
		frame->data = nullptr;
	}

	frame->header.setFrameSize(size);

	frame->data = new uint8_t[size];
	std::memcpy(frame->data, data, size);
}

void ID3v20::removeFrame(ID3v20Frame* frame){
	for(auto it = frames.begin(); it != frames.end(); ++it){
		if(*it == frame){
			delete frame;
			frames.erase(it);
			break;
		}
	}
}

std::string ID3v20::getTitle() const{
	uint8_t identifier[3] = {'T', 'T', '2'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v20::setTitle(const std::string& title){
	uint8_t identifier[3] = {'T', 'T', '2'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += title;
	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}

std::string ID3v20::getArtist() const{
	uint8_t identifier[3] = {'T', 'P', '1'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v20::setArtist(const std::string& artist){
	uint8_t identifier[3] = {'T', 'P', '1'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += artist;
	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}

std::string ID3v20::getAlbum() const{
	uint8_t identifier[3] = {'T', 'A', 'L'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v20::setAlbum(const std::string& album){
	uint8_t identifier[3] = {'T', 'A', 'L'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += album;
	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}

std::string ID3v20::getYear() const{
	uint8_t identifier[3] = {'T', 'Y', 'E'};
	ID3v20Frame* frame = getFrame(identifier);
	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return "";
		}
		return std::string(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
	}
	return "";
}

void ID3v20::setYear(const std::string& year){
	uint8_t identifier[3] = {'T', 'Y', 'E'};
	if(year.length() != 4){
		return;
	}
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += year;
	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}

std::string ID3v20::getComment() const{
	uint8_t identifier[3] = {'C', 'O', 'M'};
	ID3v20Frame* frame = getFrame(identifier);

	if(frame != nullptr){
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

	return "";
}

void ID3v20::setComment(const std::string& comment){
	uint8_t identifier[3] = {'C', 'O', 'M'};

	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += "eng"; // language
	frameData += '\0'; // empty short content description
	frameData += comment;

	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}

uint8_t ID3v20::getTrack() const{
	uint8_t identifier[3] = {'T', 'R', 'K'};
	ID3v20Frame* frame = getFrame(identifier);

	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return 0;
		}
		std::string trackString(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
		size_t slashPosition = trackString.find('/');
		if(slashPosition != std::string::npos){
			trackString = trackString.substr(0, slashPosition);
		}
		try{
			unsigned long track = std::stoul(trackString);
			if(track <= UINT8_MAX){
				return static_cast<uint8_t>(track);
			}
		}catch(...){

		}
	}

	return 0;
}

void ID3v20::setTrack(uint8_t track){
	uint8_t identifier[3] = {'T', 'R', 'K'};
	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += std::to_string(track);
	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}

ID3v10::Genre ID3v20::getGenre() const{
	uint8_t identifier[3] = {'T', 'C', 'O'};
	ID3v20Frame* frame = getFrame(identifier);

	if(frame != nullptr){
		const uint32_t frameSize = frame->header.getFrameSize();
		if(frameSize <= 1){
			return ID3v10::Genre::Other;
		}
		std::string genreString(reinterpret_cast<char*>(frame->data + 1), frameSize - 1);
		if(genreString.empty()){
			return ID3v10::Genre::Other;
		}
		if(genreString.front() == '(' && genreString.back() == ')'){
			genreString = genreString.substr(1, genreString.size() - 2);
		}
		try{
			int genreValue = std::stoi(genreString);

			if(genreValue >= 0 && genreValue <= ID3v10::HardRock){
				return static_cast<ID3v10::Genre>(genreValue);
			}
		}catch(...){

		}
	}

	return ID3v10::Genre::Other;
}

void ID3v20::setGenre(ID3v10::Genre genre){
	uint8_t identifier[3] = {'T', 'C', 'O'};

	std::string frameData;
	frameData += static_cast<char>(ISO_ENCODING_BYTE);
	frameData += '(' + std::to_string(static_cast<int>(genre)) + ')';
	setFrame(identifier, frameData.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(frameData.c_str())));
}
