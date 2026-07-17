#include "ID3v10.hpp"

#include <cstring>
#include <string>

namespace{

/**
 * @brief Calculates the number of non-null bytes in a fixed-size field.
 *
 * Iterates through the field up to @p maxLength bytes and stops at the first
 * null byte (0x00), as required by the ID3v1 tag specification.
 *
 * @param field  Pointer to the first byte of the field.
 * @param maxLength  Maximum number of bytes to inspect.
 *
 * @return Number of non-null bytes found before the first null byte or end of field.
 */
static size_t fieldLength(const uint8_t* field, size_t maxLength){
	size_t length = 0;
	while(length < maxLength && field[length] != 0){
		++length;
	}
	return length;
}

}

ID3v10::ID3v10(){
	std::memset(&data, 0, sizeof(ID3v10Data));
}

ID3v10::~ID3v10(){
	std::memset(&data, 0, sizeof(ID3v10Data));
}

std::string ID3v10::getTitle() const{
	size_t size = fieldLength(data.title, sizeof(data.title));
	return std::string(reinterpret_cast<const char*>(data.title), size);
}

void ID3v10::setTitle(const std::string& title){
	std::memset(data.title, 0, sizeof(data.title));

	size_t size = title.length() < sizeof(data.title) ? title.length() : sizeof(data.title);
	std::memcpy(data.title, title.c_str(), size);
}

std::string ID3v10::getArtist() const{
	size_t size = fieldLength(data.artist, sizeof(data.artist));
	return std::string(reinterpret_cast<const char*>(data.artist), size);
}

void ID3v10::setArtist(const std::string& artist){
	std::memset(data.artist, 0, sizeof(data.artist));

	size_t size = artist.length() < sizeof(data.artist) ? artist.length() : sizeof(data.artist);
	std::memcpy(data.artist, artist.c_str(), size);
}

std::string ID3v10::getAlbum() const{
	size_t size = fieldLength(data.album, sizeof(data.album));
	return std::string(reinterpret_cast<const char*>(data.album), size);
}

void ID3v10::setAlbum(const std::string& album){
	std::memset(data.album, 0, sizeof(data.album));

	size_t size = album.length() < sizeof(data.album) ? album.length() : sizeof(data.album);
	std::memcpy(data.album, album.c_str(), size);
}

std::string ID3v10::getYear() const{
	std::string year;
	year.push_back(static_cast<char>(data.year[0]));
	year.push_back(static_cast<char>(data.year[1]));
	year.push_back(static_cast<char>(data.year[2]));
	year.push_back(static_cast<char>(data.year[3]));
	return year;
}

void ID3v10::setYear(const std::string& year){
	std::memset(data.year, 0, sizeof(data.year));
	if(year.length() == sizeof(data.year)){
		data.year[0] = static_cast<uint8_t>(year[0]);
		data.year[1] = static_cast<uint8_t>(year[1]);
		data.year[2] = static_cast<uint8_t>(year[2]);
		data.year[3] = static_cast<uint8_t>(year[3]);
	}
}

std::string ID3v10::getComment() const{
	size_t size = fieldLength(data.comment, sizeof(data.comment));
	return std::string(reinterpret_cast<const char*>(data.comment), size);
}

void ID3v10::setComment(const std::string& comment){
	std::memset(data.comment, 0, sizeof(data.comment));

	size_t size = comment.length() < sizeof(data.comment) ? comment.length() : sizeof(data.comment);
	std::memcpy(data.comment, comment.c_str(), size);
}

ID3v10::Genre ID3v10::getGenre() const{
	return static_cast<ID3v10::Genre>(data.genre[0]);
}

void ID3v10::setGenre(ID3v10::Genre genre){
	data.genre[0] = static_cast<uint8_t>(genre);
}
