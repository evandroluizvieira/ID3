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
	size(0), flags(0), padding(0){

}

ID3v23ExtendedHeader::~ID3v23ExtendedHeader(){

}

void ID3v23ExtendedHeader::setExtendedHeaderSize(uint32_t size){
	this->size = size;
}

uint32_t ID3v23ExtendedHeader::getExtendedHeaderSize() const{
	return size;
}

void ID3v23ExtendedHeader::setExtendedFlags(uint16_t flags){
	this->flags = flags;
}

uint16_t ID3v23ExtendedHeader::getExtendedFlags() const {
    return flags;
}

void ID3v23ExtendedHeader::setPaddingSize(uint32_t size){
	this->padding = size;
}


uint32_t ID3v23ExtendedHeader::getPaddingSize() const{
	return padding;
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
}

ID3v23Frame* ID3v23::getFrame(uint8_t identifier[4]) const {
	for(auto frame : frames){
		if(std::memcmp(frame->header.identifier, identifier, 4) == 0){
			return frame;
		}
	}
	return nullptr;
}

void ID3v23::setFrame(uint8_t identifier[4], uint32_t size, uint8_t* data){
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
	if(frame != nullptr && frame->header.getFrameSize() == 4){
		return std::string(reinterpret_cast<char*>(frame->data), 4);
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
		// Convert the frame data to a string
		std::string frameData(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());

		// Read the encoding byte
		uint8_t encoding = static_cast<uint8_t>(frameData[0]);
		size_t pos = 1;

		// Extract the 3-byte language field
		std::string language = frameData.substr(pos, 3);
		pos += 3;

		// Extract the short content description (terminated by a null byte)
		size_t shortDescPos = frameData.find('\0', pos);
		if(shortDescPos == std::string::npos){
			return "";
		}

		// Move to the actual comment
		pos = shortDescPos + 1;

		if(encoding == 0x00){
			// ISO-8859-1 encoding...

			// Now extract the actual comment text
			std::string comment = frameData.substr(pos);
			return comment;
		}else if(encoding == 0x01){
			// UTF-16 encoding...
		}
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

uint8_t ID3v23::getTrack() const{
	uint8_t identifier[4] = {'T', 'R', 'C', 'K'};
	ID3v23Frame* frame = getFrame(identifier);

	if(frame != nullptr){
		// Convert frame data to string
		std::string trackString(reinterpret_cast<char*>(frame->data), frame->header.getFrameSize());

		// Find the '/' indicating the total number of tracks (if present)
		size_t slashPosition = trackString.find('/');

		// Extract the track number part
		if (slashPosition != std::string::npos){
			// Only keep the part before '/'
			trackString = trackString.substr(0, slashPosition);
		}

		// Safely convert the string to an integer and return the track number
		try{
			return std::stoul(trackString);
		}catch(...){}
	}

	return 0;
}

void ID3v23::setTrack(uint8_t track){
	uint8_t identifier[4] = {'T', 'R', 'C', 'K'};

	// Convert track number to string
	std::string trackStr = std::to_string(track);

	// Set the frame with the track number string
	setFrame(identifier, trackStr.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(trackStr.c_str())));
}

ID3v10::Genre ID3v23::getGenre() const{
	uint8_t identifier[4] = {'T', 'C', 'O', 'N'};
	ID3v23Frame* frame = getFrame(identifier);

	if (frame && frame->header.getFrameSize() > 0){
		// Convert frame data to string
		std::string genreString(reinterpret_cast<const char*>(frame->data), frame->header.getFrameSize());

		if(genreString.empty() == false){
			// Find the start and end of the genre code
			size_t start = genreString.find('(');
			if(start != std::string::npos){
				size_t end = genreString.find(')', start);
				if(end != std::string::npos && end > start + 1){
					std::string genreCode = genreString.substr(start + 1, end - start - 1);
					try{
						int genreValue = std::stoi(genreCode);
						if(genreValue >= 0 && genreValue <= 79){
							return static_cast<ID3v10::Genre>(genreValue);
						}
					}catch(...){}
				}
			}
		}
	}
	return ID3v10::Other;
}


void ID3v23::setGenre(ID3v10::Genre genre){
	uint8_t identifier[4] = {'T', 'C', 'O', 'N'};

	// Construct the genre string
	std::string genreString = "(" + std::to_string(static_cast<int>(genre)) + ")";

	// Set the frame with the genre string
	setFrame(identifier, genreString.size(), reinterpret_cast<uint8_t*>(const_cast<char*>(genreString.c_str())));
}
