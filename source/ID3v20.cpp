#include "ID3v20.hpp"

ID3v20Header::ID3v20Header() :
	ID3v2HeaderBase() {
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

uint32_t ID3v20Header::getSize() const{
	uint32_t size = 0;
	size |= static_cast<uint32_t>(this->size[0]) << 21;
	size |= static_cast<uint32_t>(this->size[1]) << 14;
	size |= static_cast<uint32_t>(this->size[2]) << 7;
	size |= static_cast<uint32_t>(this->size[3]);
	return size;
}

void ID3v20Header::setSize(uint32_t size){
	//Ensure new_size is within the 28-bit limit
	if(size > 0x0FFFFFFF){
		size = 0x0FFFFFFF;
	}

	this->size[0] = static_cast<uint8_t>((size >> 21) & 0x7F);
	this->size[1] = static_cast<uint8_t>((size >> 14) & 0x7F);
	this->size[2] = static_cast<uint8_t>((size >>  7) & 0x7F);
	this->size[3] = static_cast<uint8_t>( size        & 0x7F);
}

ID3v20::ID3v20(){

}

ID3v20::~ID3v20(){

}
