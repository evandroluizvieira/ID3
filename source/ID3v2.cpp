#include "ID3v2.hpp"
#include "ID3v20.hpp"
#include "ID3v23.hpp"
#include "ID3v24.hpp"

ID3v2HeaderBase::ID3v2HeaderBase() :
	tag{'I', 'D', '3'}, major_version(0), revision_number(0), flags(0), size{0, 0, 0, 0}{

}

ID3v2HeaderBase::~ID3v2HeaderBase(){
	tag[0] = 0;
	tag[1] = 0;
	tag[2] = 0;
	major_version = 0;
	revision_number = 0;
	flags = 0;
	size[0] = 0;
	size[1] = 0;
	size[2] = 0;
	size[3] = 0;
}

bool ID3v2HeaderBase::isUnsynchronized() const{
	return (flags & 0x80) != 0;
}

void ID3v2HeaderBase::setUnsynchronized(bool useUnsynchronization){
	if(useUnsynchronization){
		flags |= 0x80;
	} else {
		flags &= ~0x80;
	}
}

uint32_t ID3v2HeaderBase::getTagSize() const{
	uint32_t size = 0;

	// Masking the highest bit of each byte
	size |= static_cast<uint32_t>(this->size[0] & 0x7F) << 21;
	size |= static_cast<uint32_t>(this->size[1] & 0x7F) << 14;
	size |= static_cast<uint32_t>(this->size[2] & 0x7F) << 7;
	size |= static_cast<uint32_t>(this->size[3] & 0x7F);

	return size;
}

void ID3v2HeaderBase::setTagSize(uint32_t size){
	//Ensure size fits within the 28-bit limit
	if(size > 0x0FFFFFFF){
		size = 0x0FFFFFFF;
	}

	this->size[0] = static_cast<uint8_t>((size >> 21) & 0x7F);
	this->size[1] = static_cast<uint8_t>((size >> 14) & 0x7F);
	this->size[2] = static_cast<uint8_t>((size >>  7) & 0x7F);
	this->size[3] = static_cast<uint8_t>( size        & 0x7F);
}

ID3v2::ID3v2(){
	v20 = nullptr;
	v23 = nullptr;
	v24 = nullptr;
}

ID3v2::~ID3v2(){
	clear();
}

bool ID3v2::isV20() const{
	return v20 != nullptr;
}

bool ID3v2::isV23() const{
	return v23 != nullptr;
}

bool ID3v2::isV24() const{
	return v24 != nullptr;
}

void ID3v2::clear(){
	if(v20 != nullptr){
		delete v20;
		v20 = nullptr;
	}

	if(v23 != nullptr){
		delete v23;
		v23 = nullptr;
	}

	if(v24 != nullptr){
		delete v24;
		v24 = nullptr;
	}
}
