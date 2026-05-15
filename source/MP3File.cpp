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
		} else if(hasID3v23() == true){
			ID3v23* v23 = new ID3v23();

			uint32_t currentPosition = 0;

			{// Read header

				// Define fixed byte sizes from the ID3v2.3 header
				// tag[3] + major_version[1] + revision_number[1] + flags[1] + size[4]
				const uint32_t id3v2HeaderTagSizeOf = 3;
				const uint32_t id3v2HeaderMajorVersionSizeOf = 1;
				const uint32_t id3v2HeaderRevisionNumberSizeOf = 1;
				const uint32_t id3v2HeaderFlagsSizeOf = 1;
				const uint32_t id3v2HeaderTagSizeFieldSizeOf = 4;

				//Read tag
				std::memcpy(v23->header.tag, &data[currentPosition], id3v2HeaderTagSizeOf);
				currentPosition += id3v2HeaderTagSizeOf;

				//Read major version
				std::memcpy(&v23->header.major_version, &data[currentPosition], id3v2HeaderMajorVersionSizeOf);
				currentPosition += id3v2HeaderMajorVersionSizeOf;

				//Read revision_number
				std::memcpy(&v23->header.revision_number, &data[currentPosition], id3v2HeaderRevisionNumberSizeOf);
				currentPosition += id3v2HeaderRevisionNumberSizeOf;

				//Read flags
				std::memcpy(&v23->header.flags, &data[currentPosition], id3v2HeaderFlagsSizeOf);
				currentPosition += id3v2HeaderFlagsSizeOf;

				//Read tag size
				std::memcpy(v23->header.size, &data[currentPosition], id3v2HeaderTagSizeFieldSizeOf);
				currentPosition += id3v2HeaderTagSizeFieldSizeOf;
			}

			// Extended header (ID3v2.3 only if flag is set)
			if(v23->header.hasExtendedHeader() == true){

				// Define fixed byte sizes from the ID3v2.3 extended header layout:
				// size[4] + flags[2] + padding[4] + optional crc[4]
				const uint32_t id3v23ExtHeaderSizeSizeOf = 4;
				const uint32_t id3v23ExtHeaderFlagsSizeOf = 2;
				const uint32_t id3v23ExtHeaderPaddingSizeOf = 4;
				const uint32_t id3v23ExtHeaderCRCSizeOf = 4;

				v23->extendedHeader = new ID3v23ExtendedHeader();

				// Read extended header size
				std::memcpy(&v23->extendedHeader->size, &data[currentPosition], id3v23ExtHeaderSizeSizeOf);
				currentPosition += id3v23ExtHeaderSizeSizeOf;

				// Read flags
				std::memcpy(&v23->extendedHeader->flags, &data[currentPosition], id3v23ExtHeaderFlagsSizeOf);
				currentPosition += id3v23ExtHeaderFlagsSizeOf;

				// Read padding
				std::memcpy(&v23->extendedHeader->padding, &data[currentPosition], id3v23ExtHeaderPaddingSizeOf);
				currentPosition += id3v23ExtHeaderPaddingSizeOf;

				// Read CRC if present
				if(v23->extendedHeader->hasCRC() == true){
					v23->extendedHeader->crc = new uint32_t;
					std::memcpy(v23->extendedHeader->crc, &data[currentPosition], id3v23ExtHeaderCRCSizeOf);
					currentPosition += id3v23ExtHeaderCRCSizeOf;
				}
			}

			// Define fixed byte sizes from the ID3v2.3 frame header layout:
			// identifier[4] + size[4] + flags[2]
			const uint32_t id3v23FrameHeaderIdentifierSizeOf = 4;
			const uint32_t id3v23FrameHeaderSizeSizeOf = 4;
			const uint32_t id3v23FrameHeaderFlagsSizeOf = 2;
			const uint32_t id3v23FrameHeaderTotalSizeOf = id3v23FrameHeaderIdentifierSizeOf + id3v23FrameHeaderSizeSizeOf + id3v23FrameHeaderFlagsSizeOf;

			// Calculate end of tag region
			const uint32_t tagEndPosition = currentPosition + v23->header.getTagSize();

			// Iterate through frames
			while(currentPosition < tagEndPosition){
				// Check if there is enough space for a frame header
				if(currentPosition + id3v23FrameHeaderTotalSizeOf > tagEndPosition){
					break;
				}

				ID3v23Frame* frame = new ID3v23Frame();

				{// Read frame header
					// Read identifier
					std::memcpy(frame->header.identifier, &data[currentPosition], id3v23FrameHeaderIdentifierSizeOf);
					currentPosition += id3v23FrameHeaderIdentifierSizeOf;

					// Read size
					std::memcpy(frame->header.size, &data[currentPosition], id3v23FrameHeaderSizeSizeOf);
					currentPosition += id3v23FrameHeaderSizeSizeOf;

					// Read flags
					std::memcpy(&frame->header.flags, &data[currentPosition], id3v23FrameHeaderFlagsSizeOf);
					currentPosition += id3v23FrameHeaderFlagsSizeOf;
				}

				// Padding check (important for ID3v2.3)
				// ID3v2.3 tags may contain a padding area at the end of the frames section,
				// consisting of zeroed bytes (0x00). When the frame identifier is 0x000000,
				// it indicates the start of this padding region, meaning no more valid frames follow.
				if(frame->header.identifier[0] == 0x00 && frame->header.identifier[1] == 0x00 && frame->header.identifier[2] == 0x00 && frame->header.identifier[3] == 0x00){
					delete frame;
					break;
				}

				// Get frame size
				uint32_t frameSize = frame->header.getFrameSize();

				// Invalid frame size
				if(frameSize == 0){
					delete frame;
					continue;
				}

				// Frame must fit inside tag
				if(currentPosition + frameSize > tagEndPosition){
					delete frame;
					break;
				}

				// Allocate frame data
				frame->data = new uint8_t[frameSize];

				// Read frame data
				std::memcpy(frame->data, &data[currentPosition], frameSize);
				currentPosition += frameSize;

				// Store frame in ID3v23 object
				v23->frames.push_back(frame);
			}

			id3.v2.v23 = v23;
		}else if(hasID3v20() == true){
			ID3v20* v20 = new ID3v20();

			uint32_t currentPosition = 0;

			{// Read header

				// Define fixed byte sizes from the ID3v2.0 header
				// tag[3] + major_version[1] + revision_number[1] + flags[1] + size[4]
				const uint32_t id3v2HeaderTagSizeOf = 3;
				const uint32_t id3v2HeaderMajorVersionSizeOf = 1;
				const uint32_t id3v2HeaderRevisionNumberSizeOf = 1;
				const uint32_t id3v2HeaderFlagsSizeOf = 1;
				const uint32_t id3v2HeaderTagSizeFieldSizeOf = 4;

				//Read tag
				std::memcpy(v20->header.tag, &data[currentPosition], id3v2HeaderTagSizeOf);
				currentPosition += id3v2HeaderTagSizeOf;

				//Read major version
				std::memcpy(&v20->header.major_version, &data[currentPosition], id3v2HeaderMajorVersionSizeOf);
				currentPosition += id3v2HeaderMajorVersionSizeOf;

				//Read revision_number
				std::memcpy(&v20->header.revision_number, &data[currentPosition], id3v2HeaderRevisionNumberSizeOf);
				currentPosition += id3v2HeaderRevisionNumberSizeOf;

				//Read flags
				std::memcpy(&v20->header.flags, &data[currentPosition], id3v2HeaderFlagsSizeOf);
				currentPosition += id3v2HeaderFlagsSizeOf;

				//Read tag size
				std::memcpy(v20->header.size, &data[currentPosition], id3v2HeaderTagSizeFieldSizeOf);
				currentPosition += id3v2HeaderTagSizeFieldSizeOf;
			}

			// ID3v2.0 compression is not officially defined in the specification, therefore compressed tags should be ignored
			if(v20->header.isCompressed() == true){
				delete v20;
				return id3;
			}

			// Define fixed byte sizes from the ID3v2.0 frame header layout:
			// identifier[3] + size[3]
			const uint32_t id3v20FrameHeaderIdentifierSizeOf = 3;
			const uint32_t id3v20FrameHeaderSizeSizeOf = 3;
			const uint32_t id3v20FrameHeaderTotalSizeOf = id3v20FrameHeaderIdentifierSizeOf + id3v20FrameHeaderSizeSizeOf;

			// Calculate the position where frames end based on the tag size specified plus the size of the header
			const uint32_t tagEndPosition = currentPosition + v20->header.getTagSize();

			// Iterate through frames until the end of data
			while(currentPosition < tagEndPosition){
				// Check if there are enough bytes for frame header
				if(currentPosition + id3v20FrameHeaderTotalSizeOf > tagEndPosition){
					break;
				}

				ID3v20Frame* frame = new ID3v20Frame();

				{// Read frame header

					// Read identifier
					std::memcpy(frame->header.identifier, &data[currentPosition], id3v20FrameHeaderIdentifierSizeOf);
					currentPosition += id3v20FrameHeaderIdentifierSizeOf;

					// Read size
					std::memcpy(frame->header.size, &data[currentPosition], id3v20FrameHeaderSizeSizeOf);
					currentPosition += id3v20FrameHeaderSizeSizeOf;
				}

				// Padding check (important for ID3v2.0)
				// ID3v2.0 tags may contain a padding area at the end of the frames section,
				// consisting of zeroed bytes (0x00). When the frame identifier is 0x000000,
				// it indicates the start of this padding region, meaning no more valid frames follow.
				if(frame->header.identifier[0] == 0x00 && frame->header.identifier[1] == 0x00 && frame->header.identifier[2] == 0x00){
					delete frame;
					break;
				}

				// Get frame size
				uint32_t frameSize = frame->header.getFrameSize();

				// Invalid frame size
				if(frameSize == 0){
					delete frame;
					continue;
				}

				// Frame must fit inside tag
				if(currentPosition + frameSize > tagEndPosition){
					delete frame;
					break;
				}

				// Allocate frame data
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
