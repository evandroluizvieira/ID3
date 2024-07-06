#include "ID3v11.hpp"

#include <cstring>
#include <string>

ID3v11::ID3v11(){
	std::memset(&data, 0, sizeof(ID3v11Data));
}

ID3v11::~ID3v11(){
	std::memset(&data, 0, sizeof(ID3v11Data));
}

std::string ID3v11::getTitle() const{
	return std::string(reinterpret_cast<const char*>(data.title));
}

void ID3v11::setTitle(const std::string& title){
	std::memset(data.title, 0, sizeof(data.title));
	std::memcpy(data.title, title.c_str(), title.length());
}

std::string ID3v11::getArtist() const{
	return std::string(reinterpret_cast<const char*>(data.artist));
}

void ID3v11::setArtist(const std::string& artist){
	std::memset(data.artist, 0, sizeof(data.artist));
	std::memcpy(data.artist, artist.c_str(), artist.length());
}

std::string ID3v11::getAlbum() const{
	return std::string(reinterpret_cast<const char*>(data.album));
}

void ID3v11::setAlbum(const std::string& album){
	std::memset(data.album, 0, sizeof(data.album));
	std::memcpy(data.album, album.c_str(), album.length());
}

std::string ID3v11::getYear() const{
	std::string year;
	year.push_back(data.year[0]);
	year.push_back(data.year[1]);
	year.push_back(data.year[2]);
	year.push_back(data.year[3]);
	return year;
}

void ID3v11::setYear(const std::string& year){
	std::memset(data.year, 0, 4);
	if(year.length() == 4){
		data.year[0] = year[0];
		data.year[1] = year[1];
		data.year[2] = year[2];
		data.year[3] = year[3];
	}
}

std::string ID3v11::getComment() const{
	return reinterpret_cast<const char*>(data.comment);
}

void ID3v11::setComment(const std::string& comment){
	std::memset(data.comment, 0, sizeof(data.comment));
	std::memcpy(data.comment, comment.c_str(), comment.length());
}

int ID3v11::getTrack() const{
    return static_cast<int>(data.track[0]);
}

void ID3v11::setTrack(int track){
    data.track[0] = static_cast<uint8_t>(track);
}

ID3v10::Genre ID3v11::getGenre() const{
	return static_cast<ID3v10::Genre>(data.genre[0]);
}

void ID3v11::setGenre(ID3v10::Genre genre){
	data.genre[0] = static_cast<uint8_t>(genre);
}
