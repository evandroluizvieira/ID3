//Local
#include "ID3v10.hpp"
#include "ID3v11.hpp"
#include "Utils.hpp"

//Lib
#include <cstring>

//Log
#include <iostream>

#include "MP3File.hpp"
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

		if(hasID3v24() == true){
			//TODO copy id3 v24 from file 'data' to 'id3' v24
		}else if(hasID3v23() == true){
			uint8_t* dData = this->data;
			uint64_t dSize = this->size;

			ID3v2HeaderData* dHeaderData = reinterpret_cast<ID3v2HeaderData*>(&dData[0]);
			if(dHeaderData != nullptr){
				uint64_t sizeOfID3v2HeaderData = sizeof(ID3v2HeaderData);
				uint64_t sizeOfID3v23FrameHeader = sizeof(ID3v23FrameHeaderData);
				uint64_t beginBytePositionOfFramesHeader = sizeOfID3v2HeaderData;

				ID3v23* v23 = new ID3v23();
				memcpy(&v23->header.data, dHeaderData, sizeOfID3v2HeaderData);
				if(v23->header.hasExtendedHeader() == true){
					int32_t sizeOfID3v23ExtendedHeaderData = sizeof(ID3v23ExtendedHeaderData);
					int32_t sizeOfExtendedHeader = Utils::GetInt32From4UInt8(&dData[sizeOfID3v23FrameHeader]);

					v23->extendedHeader = new ID3v23ExtendedHeader();
					if(sizeOfExtendedHeader == sizeOfID3v23ExtendedHeaderData){
						memcpy(&v23->extendedHeader->data, &dData[sizeOfID3v23FrameHeader - 1], sizeOfID3v23ExtendedHeaderData);
						beginBytePositionOfFramesHeader += sizeOfID3v23ExtendedHeaderData;
					}

					if(v23->extendedHeader->hasPadding() == true){
						int paddingOfID3v23ExtendedHeaderData = 4;
						beginBytePositionOfFramesHeader += paddingOfID3v23ExtendedHeaderData;
					}
				}

				for(uint64_t i = beginBytePositionOfFramesHeader; i < dSize;){
					ID3v23FrameHeaderData* dFrameHeaderData = reinterpret_cast<ID3v23FrameHeaderData*>(&dData[i]);
					if(dFrameHeaderData == nullptr){
						continue;
					}

					int matchID = ID3v23Frame::MatchID(dFrameHeaderData->id);
					if(matchID >= 0){
						ID3v23Frame* currentFrame = new ID3v23Frame();
						memcpy(&currentFrame->header, dFrameHeaderData, sizeOfID3v23FrameHeader);
						i += sizeOfID3v23FrameHeader;

						uint64_t currentFrameDataSize = static_cast<uint64_t>(currentFrame->getSize());

						currentFrame->data = new uint8_t[currentFrameDataSize];
						memcpy(currentFrame->data, &data[i], currentFrameDataSize);
						i += currentFrameDataSize;

						v23->frames.push_back(currentFrame);
					}else{
						break;
					}
				}

				id3.v2->v23 = v23;
			}
		}else if(hasID3v22() == true){
			//TODO copy id3 v22 from file 'data' to 'id3' v22
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

	ID3v22* id3v22 = id3.v2->v22;
	ID3v23* id3v23 = id3.v2->v23;
	ID3v24* id3v24 = id3.v2->v24;

	if(id3v24 != nullptr){
		//TODO set ID3 v24 tag to 'data'
	}else if(id3v23 != nullptr){
		//TODO set ID3 v23 tag to 'data'
	}else if(id3v22 != nullptr){
		//TODO set ID3 v22 tag to 'data'
	}else{
		if(hasID3v22() == true){
			//TODO remove ID3 v22 from 'data'
		}

		if(hasID3v23() == true){
			//removing id3 v23
			ID3v2HeaderData* dHeaderData = reinterpret_cast<ID3v2HeaderData*>(&data[0]);
			if(dHeaderData != nullptr){
				uint64_t removeSize = 0;
				int sizeOfID3v2HeaderData = sizeof(ID3v2HeaderData);
				removeSize += sizeOfID3v2HeaderData;

				ID3v23 v23;
				memcpy(&v23.header.data, dHeaderData, sizeOfID3v2HeaderData);

				removeSize += v23.header.getSize();

				if(v23.header.hasExtendedHeader() == true && v23.extendedHeader != nullptr){
					removeSize += v23.extendedHeader->getSize();

					if(v23.extendedHeader->hasPadding() == true){
						removeSize += ID3v23ExtendedHeader::PaddingSize;
					}
				}

				uint64_t newSize = size - removeSize;
				uint8_t* newData = new uint8_t[newSize];
				memcpy(newData, &data[removeSize], newSize);

				clear();

				size = newSize;
				data = newData;
			}
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
