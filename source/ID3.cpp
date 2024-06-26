#include "ID3.hpp"

ID3::ID3() :
	v1(nullptr), v2(nullptr){

}

ID3::~ID3(){
	if(v1 != nullptr){
		delete v1;
		v1 = nullptr;
	}

	if(v2 != nullptr){
		delete v2;
		v2 = nullptr;
	}
}
