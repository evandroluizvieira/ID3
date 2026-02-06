#include "ID3v10.hpp"
#include "ID3v11.hpp"
#include "ID3v20.hpp"
#include "ID3v23.hpp"
#include "ID3v24.hpp"
#include "MP3File.hpp"

#include <cstring>

#include <iostream>
using std::cout;
using std::endl;

MP3File::MP3File() :
	File(){

}

MP3File::~MP3File(){

}

bool MP3File::hasID3v10() const{
	if(isEmpty() == true){
		return false;
	}

	uint64_t sizeOfID3v10Data = sizeof(ID3v10Data);
	if(size > sizeOfID3v10Data){
		uint64_t i = size - sizeOfID3v10Data;
		bool validLastCommentData = data[i + 126] == 0;
		if(data[i] == 'T' && data[i + 1] == 'A' && data[i + 2] == 'G' && validLastCommentData == true){
			return true;
		}
	}

	return false;
}

bool MP3File::hasID3v11() const{
	if(isEmpty() == true){
		return false;
	}

	uint64_t sizeOfID3v11Data = sizeof(ID3v11Data);
	if(size > sizeOfID3v11Data){
		uint64_t i = size - sizeOfID3v11Data;
		bool validLastCommentData = data[i + 125] == 0;
		bool validTrack = data[i + 126] != 0;
		if(data[i] == 'T' && data[i + 1] == 'A' && data[i + 2] == 'G' && validLastCommentData == true && validTrack == true){
			return true;
		}
	}

	return false;
}

bool MP3File::hasID3v20() const{
	if(isEmpty() == true){
		return false;
	}

	if(size >= 5 && data[0] == 'I' && data[1] == 'D' && data[2] == '3' && data[3] == 2 && data[4] == 0){
		return true;
	}else{
		return false;
	}
}

bool MP3File::hasID3v23() const{
	if(isEmpty() == true){
		return false;
	}

	if(size >= 5 && data[0] == 'I' && data[1] == 'D' && data[2] == '3' && data[3] == 3 && data[4] == 0){
		return true;
	}else{
		return false;
	}
}

bool MP3File::hasID3v24() const{
	if(isEmpty() == true){
		return false;
	}

	if(size >= 5 && data[0] == 'I' && data[1] == 'D' && data[2] == '3' && data[3] == 4 && data[4] == 0){
		return true;
	}else{
		return false;
	}
}

ID3 MP3File::getID3() const{
	ID3 id3;

	if(isEmpty() == false){
		if(hasID3v11() == true){
			int sizeOfID3v11Data = sizeof(ID3v11Data);
			ID3v11* v11 = new ID3v11();
			std::memcpy(&v11->data, &data[size - sizeOfID3v11Data], sizeOfID3v11Data);
			id3.v1.v11 = v11;
		}else if(hasID3v10() == true){
			int sizeOfID3v10Data = sizeof(ID3v10Data);
			ID3v10* v10 = new ID3v10();
			std::memcpy(&v10->data, &data[size - sizeOfID3v10Data], sizeOfID3v10Data);
			id3.v1.v10 = v10;
		}

		if(hasID3v24() == true){
			ID3v24* v24 = new ID3v24();
			//std::memcpy(&v24->header, data, sizeof(ID3v2HeaderBase));
			v24->header = *reinterpret_cast<ID3v24Header*>(data);

			id3.v2.v24 = v24;
		}else if(hasID3v23() == true){
			ID3v23* v23 = new ID3v23();
			//std::memcpy(&v23->header, data, sizeof(ID3v2HeaderBase));
			v23->header = *reinterpret_cast<ID3v23Header*>(data);

			id3.v2.v23 = v23;
		}else if(hasID3v20() == true){
			ID3v20* v20 = new ID3v20();
			//std::memcpy(&v20->header, data, sizeof(ID3v2HeaderBase));
			v20->header = *reinterpret_cast<ID3v20Header*>(data);

			id3.v2.v20 = v20;

		uint32_t tagSize = v20->header.getTagSize();

			// Determine the position where frames start after the header
			uint32_t headerBaseSize = sizeof(ID3v2HeaderBase);
			uint32_t currentPosition = headerBaseSize;

			// Iterate through frames until the end of data
			while(currentPosition < tagSize){
				ID3v20Frame* frame = new ID3v20Frame();

				// Read frame header
				//std::memcpy(&frame->header, &data[currentPosition], sizeof(ID3v20FrameHeader));
				frame->header = *reinterpret_cast<ID3v20FrameHeader*>(&data[currentPosition]);
				currentPosition += sizeof(ID3v20FrameHeader);

				// Determine frame size and allocate memory for frame data
				uint32_t frameSize = frame->header.getFrameSize();
				frame->data = new uint8_t[frameSize];

				// Read frame data
				std::memcpy(frame->data, &data[currentPosition], frameSize);
				currentPosition += frameSize;

				// Store frame in ID3v20 object
				v20->frames.push_back(frame);
			}

			id3.v2.v20 = v20;
		}
	}

	return id3;
}

void MP3File::setID3(const ID3& id3){
	if(isEmpty() == true){
		return;
	}

	ID3v10* id3v10 = id3.v1.v10;
	ID3v11* id3v11 = id3.v1.v11;

	if(id3v11 != nullptr){
		uint64_t sizeOfID3v11Data = sizeof(ID3v11Data);
		uint64_t newSize = size + sizeOfID3v11Data;
		uint8_t* newData = new uint8_t[newSize];

		std::memcpy(newData, data, size);
		std::memcpy(&newData[size - 1], &id3v11->data, sizeOfID3v11Data);

		clear();

		data = newData;
		size = newSize;
	}else if(id3v10 != nullptr){
		uint64_t sizeOfID3v10Data = sizeof(ID3v10Data);
		uint64_t newSize = size + sizeOfID3v10Data;
		uint8_t* newData = new uint8_t[newSize];

		std::memcpy(newData, data, size);
		std::memcpy(&newData[size - 1], &id3v10->data, sizeOfID3v10Data);

		clear();

		data = newData;
		size = newSize;
	}else{
		//removing id3 v10 or v11 from 'data'
		if(hasID3v10() == true || hasID3v11() == true){
			uint64_t sizeOfID3v10Data = sizeof(ID3v10Data);
			uint64_t newSize = size - sizeOfID3v10Data;
			uint8_t* newData = new uint8_t[newSize];

			std::memcpy(newData, data, newSize);

			clear();

			data = newData;
			size = newSize;
		}
	}

	ID3v20* id3v20 = id3.v2.v20;
	ID3v23* id3v23 = id3.v2.v23;
	ID3v24* id3v24 = id3.v2.v24;

	if(id3v24 != nullptr){
		//TODO set ID3 v24 tag to 'data'
	}else if(id3v23 != nullptr){
		//TODO set ID3 v23 tag to 'data'
	}else if(id3v20 != nullptr){
		//TODO set ID3 v20 tag to 'data'
	}else{
		if(hasID3v20() == true){
			//TODO remove ID3 v20 from 'data'
		}

		if(hasID3v23() == true){
			//TODO remove ID3 v23 from 'data'
		}

		if(hasID3v24() == true){
			//TODO remove ID3 v24 from 'data'
		}
	}
}

void MP3File::removeTags(){
	ID3 id3;
	setID3(id3);
}
