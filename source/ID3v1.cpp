#include "ID3v1.hpp"

ID3v1::ID3v1(){
    v10 = nullptr;
    v11 = nullptr;

}

ID3v1::~ID3v1(){
    remove();
}

bool ID3v1::isV10() const{
    return v10 != nullptr;
}

bool ID3v1::isV11() const{
    return v11 != nullptr;
}

void ID3v1::remove(){
    if(isV10() == true){
        delete v10;
        v10 = nullptr;
    }
    if(isV11() == true){
        delete v11;
        v11 = nullptr;
    }
}
