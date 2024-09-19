#include "File.hpp"

#include <fstream>
#include <stdexcept>

File::File() :
	data(nullptr), size(0){

}

File::~File(){
	clear();
}

void File::clear(){
	if(data != nullptr){
		delete[] data;
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

uint8_t& File::operator[](uint64_t index) {
	if(index >= size){
		throw std::out_of_range("Index out of range");
	}else{
		return data[index];
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

	std::streamsize fileSize = file.tellg();
	if(fileSize <= 0){
		return false;
	}

	file.seekg(0, std::ios::beg);
	if(file.fail() == true){
		return false;
	}

	data = new uint8_t[static_cast<std::size_t>(fileSize)];

	file.read(reinterpret_cast<char*>(data), fileSize);
	if(file.fail() == true){
		clear();
		return false;
	}

	size = static_cast<uint64_t>(fileSize);

	return true;
}

bool File::save(const char* filename){
	if(isEmpty() == true){
		return false;
	}

	std::ofstream file(filename, std::ios::out | std::ios::binary);
	if(file.is_open() == false){
		return false;
	}

	file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
	if(file.fail() == true){
		return false;
	}

	return true;
}

