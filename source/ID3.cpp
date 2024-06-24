#include "ID3.hpp"

ID3::ID3() :
	v1(nullptr){

}

ID3::~ID3(){
	if(v1 != nullptr){
		delete v1;
		v1 = nullptr;
	}
}
