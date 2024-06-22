#include "File.hpp"

#include <fstream>

File::File() :
	data(nullptr), size(nullptr){

}

File::~File(){
	clear();
}

void File::clear(){
	if(data != nullptr){
		delete data;
		data = nullptr;
	}
	size = 0;
}

bool File::isEmpty() const{
	if(data == nullptr || size == 0){
		return true;
	}else{
		return false;
	}
}

bool File::load(const char *filename){
	if(isEmpty() == false){
		return false;
	}

	std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if(file.is_open() == false){
		return false;
	}

	char *buffer = nullptr;
	long long size = 0;
	size = file.tellg();
	file.seekg(0, std::ios::beg);
	buffer = new char[static_cast<unsigned long long>(size)];
	file.read(buffer, size);
	file.close();

	this->data = reinterpret_cast<uint8_t*>(buffer);
	this->size = static_cast<uint64_t>(size);

	return true;
}

bool File::save(const char *filename){
	if(isEmpty() == true){
		return false;
	}

	std::ofstream file(filename, std::ios::out | std::ios::binary);
	if(file.is_open() == false){
		return false;
	}

	file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
	file.close();

	return true;
}
