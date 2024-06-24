#include "ID3v10.hpp"
#include "ID3v11.hpp"
#include "MP3File.hpp"

#include <cstring>

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

bool MP3File::hasID3v22() const{
	if(isEmpty() == true){
		return false;
	}

	if(size >= 4 && data[0] == 'I' && data[1] == 'D' && data[2] == '3' && data[3] == 2){
		return true;
	}else{
		return false;
	}
}

bool MP3File::hasID3v23() const{
	if(isEmpty() == true){
		return false;
	}

	if(size >= 4 && data[0] == 'I' && data[1] == 'D' && data[2] == '3' && data[3] == 3){
		return true;
	}else{
		return false;
	}
}

bool MP3File::hasID3v24() const{
	if(isEmpty() == true){
		return false;
	}

	if(size >= 4 && data[0] == 'I' && data[1] == 'D' && data[2] == '3' && data[3] == 4){
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
			memcpy(&v11->data, &data[size - sizeOfID3v11Data], sizeOfID3v11Data);
			id3.v1->v11 = v11;
		}else if(hasID3v10() == true){
			int sizeOfID3v10Data = sizeof(ID3v10Data);
			ID3v10* v10 = new ID3v10();
			memcpy(&v10->data, &data[size - sizeOfID3v10Data], sizeOfID3v10Data);
			id3.v1->v10 = v10;
		}
	}

	return id3;
}

void MP3File::setID3(const ID3& id3){
	if(isEmpty() == true){
		return;
	}

	ID3v10* id3v10 = id3.v1->v10;
	ID3v11* id3v11 = id3.v1->v11;

	if(id3v11 != nullptr){
		uint64_t sizeOfID3v11Data = sizeof(ID3v11Data);
		uint64_t newSize = size + sizeOfID3v11Data;
		uint8_t* newData = new uint8_t[newSize];

		memcpy(newData, data, size);
		memcpy(&newData[size - 1], &id3v11->data, sizeOfID3v11Data);

		clear();

		data = newData;
		size = newSize;
	}else if(id3v10 != nullptr){
		uint64_t sizeOfID3v10Data = sizeof(ID3v10Data);
		uint64_t newSize = size + sizeOfID3v10Data;
		uint8_t* newData = new uint8_t[newSize];

		memcpy(newData, data, size);
		memcpy(&newData[size - 1], &id3v10->data, sizeOfID3v10Data);

		clear();

		data = newData;
		size = newSize;
	}else{
		//removing id3 v10 or v11 from 'data'
		if(hasID3v10() == true || hasID3v11() == true){
			uint64_t sizeOfID3v10Data = sizeof(ID3v10Data);
			uint64_t newSize = size - sizeOfID3v10Data;
			uint8_t* newData = new uint8_t[newSize];

			memcpy(newData, data, newSize);

			clear();

			data = newData;
			size = newSize;
		}
	}
}

void MP3File::removeTags(){
	ID3 id3;
	setID3(id3);
}
