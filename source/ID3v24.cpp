#include "ID3v24.hpp"

ID3v24Header::ID3v24Header() :
	ID3v23Header(){
	major_version = 4;
}

ID3v24Header::~ID3v24Header(){

}

void ID3v24Header::setFooterPresent(bool hasFooter){
	if(hasFooter){
		flags |= 0x10;
	}else{
		flags &= ~0x10;
	}
}

bool ID3v24Header::hasFooter() const{
	return (flags & 0x10) != 0;
}

ID3v24::ID3v24(){

}

ID3v24::~ID3v24(){

}
