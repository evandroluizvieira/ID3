#include "ID3v23.hpp"

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

ID3v23::ID3v23(){

}

ID3v23::~ID3v23(){

}
