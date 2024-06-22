#include "ID3v2.hpp"

ID3v2::ID3v2() :
    v22(nullptr), v23(nullptr), v24(nullptr){

}

ID3v2::~ID3v2(){
    remove();
}

bool ID3v2::hasV22() const{
    return v22 != nullptr;
}

void ID3v2::removeV22(){
    if(isV22() == true){
        delete v22;
        v22 = nullptr;
    }
}

bool ID3v2::isV23() const{
    return v23 != nullptr;
}

bool ID3v2::isV24() const{
    return v24 != nullptr;
}

void ID3v2::remove(){
    if(v22 != nullptr){
        delete v22;
        v22 = nullptr;
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
