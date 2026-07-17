#include "ID3v10.hpp"
#include "ID3v11.hpp"
#include "ID3v20.hpp"
#include "ID3v23.hpp"
#include "ID3v24.hpp"
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
    if(size >= sizeOfID3v10Data){
        uint64_t i = size - sizeOfID3v10Data;
        bool validHeader = data[i] == 'T' && data[i + 1] == 'A' && data[i + 2] == 'G';
        bool isV11 = data[i + 125] == 0 && data[i + 126] != 0;

        if(validHeader == true && isV11 == false){
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
    if(size >= sizeOfID3v11Data){
        uint64_t i = size - sizeOfID3v11Data;
        bool validHeader = data[i] == 'T' && data[i + 1] == 'A' && data[i + 2] == 'G';
        bool validLastCommentData = data[i + 125] == 0;
        bool validTrack = data[i + 126] != 0;

        if(validHeader == true && validLastCommentData == true && validTrack == true){
            return true;
        }
    }

    return false;
}

bool MP3File::hasID3v20() const{
    if(isEmpty() == true){
        return false;
    }

    bool hasHeaderSize = size >= 10;
    if(!hasHeaderSize){
        return false;
    }

    bool hasID3Tag = data[0] == 'I' && data[1] == 'D' && data[2] == '3';
    bool isV20 = data[3] == 2 && data[4] == 0;
    return hasID3Tag && isV20;
}

bool MP3File::hasID3v23() const{
    if(isEmpty() == true){
        return false;
    }

    bool hasHeaderSize = size >= 10;
    if(!hasHeaderSize){
        return false;
    }

    bool hasID3Tag = data[0] == 'I' && data[1] == 'D' && data[2] == '3';
    bool isV23 = data[3] == 3 && data[4] == 0;
    return hasID3Tag && isV23;
}

bool MP3File::hasID3v24() const{
    if(isEmpty() == true){
        return false;
    }

    bool hasHeaderSize = size >= 10;
    if(!hasHeaderSize){
        return false;
    }

    bool hasID3Tag = data[0] == 'I' && data[1] == 'D' && data[2] == '3';
    bool isV24 = data[3] == 4 && data[4] == 0;
    return hasID3Tag && isV24;
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

			uint32_t currentPosition = 0;

			{// Read header

				// Define fixed byte sizes from the ID3v2.4 header
				// tag[3] + major_version[1] + revision_number[1] + flags[1] + size[4]
				const uint32_t id3v2HeaderTagSizeOf = 3;
				const uint32_t id3v2HeaderMajorVersionSizeOf = 1;
				const uint32_t id3v2HeaderRevisionNumberSizeOf = 1;
				const uint32_t id3v2HeaderFlagsSizeOf = 1;
				const uint32_t id3v2HeaderTagSizeFieldSizeOf = 4;

				//Read tag
				std::memcpy(v24->header.tag, &data[currentPosition], id3v2HeaderTagSizeOf);
				currentPosition += id3v2HeaderTagSizeOf;

				//Read major version
				std::memcpy(&v24->header.major_version, &data[currentPosition], id3v2HeaderMajorVersionSizeOf);
				currentPosition += id3v2HeaderMajorVersionSizeOf;

				//Read revision_number
				std::memcpy(&v24->header.revision_number, &data[currentPosition], id3v2HeaderRevisionNumberSizeOf);
				currentPosition += id3v2HeaderRevisionNumberSizeOf;

				//Read flags
				std::memcpy(&v24->header.flags, &data[currentPosition], id3v2HeaderFlagsSizeOf);
				currentPosition += id3v2HeaderFlagsSizeOf;

				//Read tag size (synchsafe)
				std::memcpy(v24->header.size, &data[currentPosition], id3v2HeaderTagSizeFieldSizeOf);
				currentPosition += id3v2HeaderTagSizeFieldSizeOf;
			}

			// Extended header (ID3v2.4 only if flag is set)
			if(v24->header.hasExtendedHeader() == true){
				v24->extendedHeader = new ID3v24ExtendedHeader();

				// Read extended header size (4 bytes synchsafe)
				uint8_t extSizeSynch[4];
				std::memcpy(extSizeSynch, &data[currentPosition], 4);
				currentPosition += 4;

				// Convert synchsafe to plain integer (rest size after these 4 bytes)
				uint32_t extSize = 0;
				extSize |= static_cast<uint32_t>(extSizeSynch[0]) << 21;
				extSize |= static_cast<uint32_t>(extSizeSynch[1]) << 14;
				extSize |= static_cast<uint32_t>(extSizeSynch[2]) << 7;
				extSize |= extSizeSynch[3];
				v24->extendedHeader->size = extSize;

				// Number of flag bytes (must be 1)
				std::memcpy(&v24->extendedHeader->flagBytes, &data[currentPosition], 1);
				currentPosition += 1;

				// Extended flags byte
				std::memcpy(&v24->extendedHeader->flags, &data[currentPosition], 1);
				currentPosition += 1;

				// Flag b - update (has a length byte, always 0)
				if(v24->extendedHeader->isUpdate() == true){
					uint8_t updateLen = 0;
					std::memcpy(&updateLen, &data[currentPosition], 1);
					currentPosition += 1;
				}

				// Flag c - CRC data present
				if(v24->extendedHeader->hasCRC() == true){
					uint8_t crcLen = 0;
					std::memcpy(&crcLen, &data[currentPosition], 1);
					currentPosition += 1;
					if(crcLen == 5){
						uint8_t crcSynch[5];
						std::memcpy(crcSynch, &data[currentPosition], 5);
						currentPosition += 5;
						// Convert 35-bit synchsafe to 32-bit integer
						uint32_t crcVal = 0;
						crcVal |= static_cast<uint32_t>(crcSynch[0]) << 28;
						crcVal |= static_cast<uint32_t>(crcSynch[1]) << 21;
						crcVal |= static_cast<uint32_t>(crcSynch[2]) << 14;
						crcVal |= static_cast<uint32_t>(crcSynch[3]) << 7;
						crcVal |= crcSynch[4];
						v24->extendedHeader->setCRC(crcVal);
					} else {
						v24->extendedHeader->removeCRC();
					}
				}

				// Flag d - restrictions (skip)
				if((v24->extendedHeader->flags & 0x10) != 0){
					uint8_t restrictionLen = 0;
					std::memcpy(&restrictionLen, &data[currentPosition], 1);
					currentPosition += 1;
					currentPosition += restrictionLen;
				}

				// Ensure correct position (extended header total size = 4 + extSize)
				currentPosition = 10 + 4 + extSize;
			}

			// Define fixed byte sizes from the ID3v2.4 frame header layout:
			// identifier[4] + size[4] + flags[2]
			const uint32_t id3v24FrameHeaderIdentifierSizeOf = 4;
			const uint32_t id3v24FrameHeaderSizeSizeOf = 4;
			const uint32_t id3v24FrameHeaderFlagsSizeOf = 2;
			const uint32_t id3v24FrameHeaderTotalSizeOf = id3v24FrameHeaderIdentifierSizeOf + id3v24FrameHeaderSizeSizeOf + id3v24FrameHeaderFlagsSizeOf;

			// Calculate end of tag region (adjust for footer if present)
			uint32_t tagEndPosition = currentPosition + v24->header.getTagSize();
			if(v24->header.hasFooter() == true){
				tagEndPosition -= 10; // footer occupies last 10 bytes
			}

			// Iterate through frames
			while(currentPosition < tagEndPosition){
				// Check if there is enough space for a frame header
				if(currentPosition + id3v24FrameHeaderTotalSizeOf > tagEndPosition){
					break;
				}

				ID3v24Frame* frame = new ID3v24Frame();

				{// Read frame header
					// Read identifier
					std::memcpy(frame->header.identifier, &data[currentPosition], id3v24FrameHeaderIdentifierSizeOf);
					currentPosition += id3v24FrameHeaderIdentifierSizeOf;

					// Read size (synchsafe)
					std::memcpy(frame->header.size, &data[currentPosition], id3v24FrameHeaderSizeSizeOf);
					currentPosition += id3v24FrameHeaderSizeSizeOf;

					// Read flags
					std::memcpy(frame->header.flags, &data[currentPosition], id3v24FrameHeaderFlagsSizeOf);
					currentPosition += id3v24FrameHeaderFlagsSizeOf;
				}

				// Padding check (important for ID3v2.4)
				// When the frame identifier is all zeros, it indicates the start of padding
				if(frame->header.identifier[0] == 0x00 && frame->header.identifier[1] == 0x00 &&
				frame->header.identifier[2] == 0x00 && frame->header.identifier[3] == 0x00){
					delete frame;
					break;
				}

				// Get frame size (decoded from synchsafe)
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

				// Store frame in ID3v24 object
				v24->frames.push_back(frame);
			}

			// Read footer if present
			if(v24->header.hasFooter() == true){
				v24->footer = new ID3v24Footer();
				const uint32_t footerOffset = 10 + v24->header.getTagSize(); // start of footer
				std::memcpy(v24->footer->identifier, &data[footerOffset], 3);
				v24->footer->major_version = data[footerOffset + 3];
				v24->footer->revision = data[footerOffset + 4];
				v24->footer->flags = data[footerOffset + 5];
				std::memcpy(v24->footer->size, &data[footerOffset + 6], 4);
			}

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
		std::memcpy(&newData[size], &id3v11->data, sizeOfID3v11Data);

		clear();

		data = newData;
		size = newSize;
	}else if(id3v10 != nullptr){
		uint64_t sizeOfID3v10Data = sizeof(ID3v10Data);
		uint64_t newSize = size + sizeOfID3v10Data;
		uint8_t* newData = new uint8_t[newSize];

		std::memcpy(newData, data, size);
		std::memcpy(&newData[size], &id3v10->data, sizeOfID3v10Data);

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
		// Remove any existing ID3v2 tag first
		if(hasID3v24() || hasID3v23() || hasID3v20()){
			uint32_t tagSize = 0;
			if(hasID3v24()){
				uint8_t sizeSynch[4];
				std::memcpy(sizeSynch, &data[6], 4);
				tagSize = (sizeSynch[0] << 21) | (sizeSynch[1] << 14) | (sizeSynch[2] << 7) | sizeSynch[3];
				tagSize += 10;
			}else if(hasID3v23()){
				uint8_t sizeBytes[4];
				std::memcpy(sizeBytes, &data[6], 4);
				tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
				tagSize += 10;
			}else if(hasID3v20()){
				uint8_t sizeBytes[4];
				std::memcpy(sizeBytes, &data[6], 4);
				tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
				tagSize += 10;
			}
			if(tagSize < size){
				uint64_t newSize = size - tagSize;
				uint8_t* newData = new uint8_t[newSize];
				std::memcpy(newData, &data[tagSize], newSize);
				clear();
				data = newData;
				size = newSize;
			}
		}

		// Build tag data size
		uint32_t tagDataSize = 0;
		if(id3v24->extendedHeader) tagDataSize += id3v24->extendedHeader->getSize();
		for(auto& f : id3v24->frames) tagDataSize += 10 + f->header.getFrameSize();
		if(id3v24->header.hasFooter()) tagDataSize += 10;

		// Total tag size including header
		uint32_t totalTagSize = 10 + tagDataSize;

		// Allocate new buffer
		uint64_t newSize = size + totalTagSize;
		uint8_t* newData = new uint8_t[newSize];

		// Write header (synchsafe size)
		std::memcpy(&newData[0], id3v24->header.tag, 3);
		newData[3] = id3v24->header.major_version;
		newData[4] = id3v24->header.revision_number;
		newData[5] = id3v24->header.flags;
		uint8_t sizeSynch[4];
		sizeSynch[0] = (tagDataSize >> 21) & 0x7F;
		sizeSynch[1] = (tagDataSize >> 14) & 0x7F;
		sizeSynch[2] = (tagDataSize >> 7) & 0x7F;
		sizeSynch[3] = tagDataSize & 0x7F;
		std::memcpy(&newData[6], sizeSynch, 4);

		uint32_t pos = 10;

		// Extended header
		if(id3v24->extendedHeader){
			uint32_t extSize = id3v24->extendedHeader->size;
			uint8_t extSynch[4];
			extSynch[0] = (extSize >> 21) & 0x7F;
			extSynch[1] = (extSize >> 14) & 0x7F;
			extSynch[2] = (extSize >> 7) & 0x7F;
			extSynch[3] = extSize & 0x7F;
			std::memcpy(&newData[pos], extSynch, 4);
			pos += 4;
			newData[pos++] = id3v24->extendedHeader->flagBytes;
			newData[pos++] = id3v24->extendedHeader->flags;
			if(id3v24->extendedHeader->isUpdate()) newData[pos++] = 0;
			if(id3v24->extendedHeader->hasCRC()){
				uint32_t crc = id3v24->extendedHeader->getCRC();
				uint8_t crcSynch[5];
				crcSynch[0] = (crc >> 28) & 0x7F;
				crcSynch[1] = (crc >> 21) & 0x7F;
				crcSynch[2] = (crc >> 14) & 0x7F;
				crcSynch[3] = (crc >> 7) & 0x7F;
				crcSynch[4] = crc & 0x7F;
				std::memcpy(&newData[pos], crcSynch, 5);
				pos += 5;
			}
			if((id3v24->extendedHeader->flags & 0x10) != 0){
				newData[pos++] = 0; // restriction length placeholder
			}
		}

		// Frames
		for(auto& f : id3v24->frames){
			std::memcpy(&newData[pos], f->header.identifier, 4);
			pos += 4;
			uint32_t fsize = f->header.getFrameSize();
			uint8_t fsizeSynch[4];
			fsizeSynch[0] = (fsize >> 21) & 0x7F;
			fsizeSynch[1] = (fsize >> 14) & 0x7F;
			fsizeSynch[2] = (fsize >> 7) & 0x7F;
			fsizeSynch[3] = fsize & 0x7F;
			std::memcpy(&newData[pos], fsizeSynch, 4);
			pos += 4;
			std::memcpy(&newData[pos], f->header.flags, 2);
			pos += 2;
			std::memcpy(&newData[pos], f->data, fsize);
			pos += fsize;
		}

		// Footer
		if(id3v24->header.hasFooter()){
			std::memcpy(&newData[pos], id3v24->footer->identifier, 3);
			pos += 3;
			newData[pos++] = id3v24->footer->major_version;
			newData[pos++] = id3v24->footer->revision;
			newData[pos++] = id3v24->footer->flags;
			std::memcpy(&newData[pos], id3v24->footer->size, 4);
			pos += 4;
		}

		// Append original audio data
		std::memcpy(&newData[totalTagSize], data, size);

		clear();
		data = newData;
		size = newSize;
	}else if(id3v23 != nullptr){
		if(hasID3v24() || hasID3v23() || hasID3v20()){
			uint32_t tagSize = 0;
			if(hasID3v24()){
				uint8_t sizeSynch[4];
				std::memcpy(sizeSynch, &data[6], 4);
				tagSize = (sizeSynch[0] << 21) | (sizeSynch[1] << 14) | (sizeSynch[2] << 7) | sizeSynch[3];
				tagSize += 10;
			}else if(hasID3v23()){
				uint8_t sizeBytes[4];
				std::memcpy(sizeBytes, &data[6], 4);
				tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
				tagSize += 10;
			}else if(hasID3v20()){
				uint8_t sizeBytes[4];
				std::memcpy(sizeBytes, &data[6], 4);
				tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
				tagSize += 10;
			}
			if(tagSize < size){
				uint64_t newSize = size - tagSize;
				uint8_t* newData = new uint8_t[newSize];
				std::memcpy(newData, &data[tagSize], newSize);
				clear();
				data = newData;
				size = newSize;
			}
		}

		uint32_t tagDataSize = 0;
		if(id3v23->extendedHeader) tagDataSize += id3v23->extendedHeader->getSize();
		for(auto& f : id3v23->frames) tagDataSize += 10 + f->header.getFrameSize();

		uint32_t totalTagSize = 10 + tagDataSize;
		uint64_t newSize = size + totalTagSize;
		uint8_t* newData = new uint8_t[newSize];

		// Header (normal size)
		std::memcpy(&newData[0], id3v23->header.tag, 3);
		newData[3] = id3v23->header.major_version;
		newData[4] = id3v23->header.revision_number;
		newData[5] = id3v23->header.flags;
		uint8_t sz[4];
		sz[0] = (tagDataSize >> 24) & 0xFF;
		sz[1] = (tagDataSize >> 16) & 0xFF;
		sz[2] = (tagDataSize >> 8) & 0xFF;
		sz[3] = tagDataSize & 0xFF;
		std::memcpy(&newData[6], sz, 4);

		uint32_t pos = 10;

		if(id3v23->extendedHeader){
			uint32_t extSize = id3v23->extendedHeader->size;
			uint8_t esz[4];
			esz[0] = (extSize >> 24) & 0xFF;
			esz[1] = (extSize >> 16) & 0xFF;
			esz[2] = (extSize >> 8) & 0xFF;
			esz[3] = extSize & 0xFF;
			std::memcpy(&newData[pos], esz, 4);
			pos += 4;
			std::memcpy(&newData[pos], &id3v23->extendedHeader->flags, 2);
			pos += 2;
			std::memcpy(&newData[pos], &id3v23->extendedHeader->padding, 4);
			pos += 4;
			if(id3v23->extendedHeader->hasCRC()){
				std::memcpy(&newData[pos], id3v23->extendedHeader->crc, 4);
				pos += 4;
			}
		}

		for(auto& f : id3v23->frames){
			std::memcpy(&newData[pos], f->header.identifier, 4);
			pos += 4;
			uint32_t fsize = f->header.getFrameSize();
			uint8_t fs[4];
			fs[0] = (fsize >> 24) & 0xFF;
			fs[1] = (fsize >> 16) & 0xFF;
			fs[2] = (fsize >> 8) & 0xFF;
			fs[3] = fsize & 0xFF;
			std::memcpy(&newData[pos], fs, 4);
			pos += 4;
			std::memcpy(&newData[pos], f->header.flags, 2);
			pos += 2;
			std::memcpy(&newData[pos], f->data, fsize);
			pos += fsize;
		}

		std::memcpy(&newData[totalTagSize], data, size);
		clear();
		data = newData;
		size = newSize;
	}else if(id3v20 != nullptr){
		if(hasID3v24() || hasID3v23() || hasID3v20()){
			uint32_t tagSize = 0;
			if(hasID3v24()){
				uint8_t sizeSynch[4];
				std::memcpy(sizeSynch, &data[6], 4);
				tagSize = (sizeSynch[0] << 21) | (sizeSynch[1] << 14) | (sizeSynch[2] << 7) | sizeSynch[3];
				tagSize += 10;
			}else if(hasID3v23()){
				uint8_t sizeBytes[4];
				std::memcpy(sizeBytes, &data[6], 4);
				tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
				tagSize += 10;
			}else if(hasID3v20()){
				uint8_t sizeBytes[4];
				std::memcpy(sizeBytes, &data[6], 4);
				tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
				tagSize += 10;
			}
			if(tagSize < size){
				uint64_t newSize = size - tagSize;
				uint8_t* newData = new uint8_t[newSize];
				std::memcpy(newData, &data[tagSize], newSize);
				clear();
				data = newData;
				size = newSize;
			}
		}

		uint32_t tagDataSize = 0;
		for(auto& f : id3v20->frames) tagDataSize += 6 + f->header.getFrameSize();
		uint32_t totalTagSize = 10 + tagDataSize;
		uint64_t newSize = size + totalTagSize;
		uint8_t* newData = new uint8_t[newSize];

		std::memcpy(&newData[0], id3v20->header.tag, 3);
		newData[3] = id3v20->header.major_version;
		newData[4] = id3v20->header.revision_number;
		newData[5] = id3v20->header.flags;
		uint8_t sz[4];
		sz[0] = (tagDataSize >> 24) & 0xFF;
		sz[1] = (tagDataSize >> 16) & 0xFF;
		sz[2] = (tagDataSize >> 8) & 0xFF;
		sz[3] = tagDataSize & 0xFF;
		std::memcpy(&newData[6], sz, 4);

		uint32_t pos = 10;
		for(auto& f : id3v20->frames){
			std::memcpy(&newData[pos], f->header.identifier, 3);
			pos += 3;
			uint32_t fsize = f->header.getFrameSize();
			newData[pos++] = (fsize >> 16) & 0xFF;
			newData[pos++] = (fsize >> 8) & 0xFF;
			newData[pos++] = fsize & 0xFF;
			std::memcpy(&newData[pos], f->data, fsize);
			pos += fsize;
		}

		std::memcpy(&newData[totalTagSize], data, size);
		clear();
		data = newData;
		size = newSize;
	}else{
		if(hasID3v20()){
			// Remove any ID3v20 tag present
			uint8_t sizeBytes[4];
			std::memcpy(sizeBytes, &data[6], 4);
			uint32_t tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
			tagSize += 10;
			if(tagSize < size){
				uint64_t newSize = size - tagSize;
				uint8_t* newData = new uint8_t[newSize];
				std::memcpy(newData, &data[tagSize], newSize);
				clear();
				data = newData;
				size = newSize;
			}
		}

		if(hasID3v23()){
			// Remove any ID3v23 tag present
			uint8_t sizeBytes[4];
			std::memcpy(sizeBytes, &data[6], 4);
			uint32_t tagSize = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
			tagSize += 10;
			if(tagSize < size){
				uint64_t newSize = size - tagSize;
				uint8_t* newData = new uint8_t[newSize];
				std::memcpy(newData, &data[tagSize], newSize);
				clear();
				data = newData;
				size = newSize;
			}
		}

		if(hasID3v24()){
			// Remove any ID3v24 tag present
			uint8_t sizeSynch[4];
			std::memcpy(sizeSynch, &data[6], 4);
			uint32_t tagSize = (sizeSynch[0] << 21) | (sizeSynch[1] << 14) | (sizeSynch[2] << 7) | sizeSynch[3];
			tagSize += 10;
			if(tagSize < size){
				uint64_t newSize = size - tagSize;
				uint8_t* newData = new uint8_t[newSize];
				std::memcpy(newData, &data[tagSize], newSize);
				clear();
				data = newData;
				size = newSize;
			}
		}
	}
}

void MP3File::removeTags(){
	ID3 id3;
	setID3(id3);
}
