#include "ID3v10.hpp"

#include <cstring>
#include <string>

std::string ID3v10::GetGenreAsString(ID3v10::Genre genre){
	switch(genre){
		case Blues: 			return "Blues";
		case ClassicRock:		return "Classic Rock";
		case Country:			return "Country";
		case Dance:				return "Dance";
		case Disco:				return "Disco";
		case Funk:				return "Funk";
		case Grunge:			return "Grunge";
		case HipHop:			return "Hip-Hop";
		case Jazz:				return "Jazz";
		case Metal:				return "Metal";
		case NewAge:			return "New Age";
		case Oldies:			return "Oldies";
		case Other:				return "Other";
		case Pop:				return "Pop";
		case RAndB:				return "R&B";
		case Rap:				return "Rap";
		case Reggae:			return "Reggae";
		case Rock:				return "Rock";
		case Techno:			return "Techno";
		case Industrial:		return "Industrial";
		case Alternative:		return "Alternative";
		case Ska:				return "Ska";
		case DeathMetal:		return "Death Metal";
		case Pranks:			return "Pranks";
		case Soundtrack:		return "Soundtrack";
		case EuroTechno:		return "Euro-Techno";
		case Ambient:			return "Ambient";
		case TripHop:			return "Trip-Hop";
		case Vocal:				return "Vocal";
		case JazzFunk:			return "Jazz+Funk";
		case Fusion:			return "Fusion";
		case Trance:			return "Trance";
		case Classical:			return "Classical";
		case Instrumental:		return "Instrumental";
		case Acid:				return "Acid";
		case House:				return "House";
		case Game:				return "Game";
		case SoundClip:			return "Sound Clip";
		case Gospel:			return "Gospel";
		case Noise:				return "Noise";
		case AlternRock:		return "AlternRock";
		case Bass:				return "Bass";
		case Soul:				return "Soul";
		case Punk:				return "Punk";
		case Space:				return "Space";
		case Meditative:		return "Meditative";
		case InstrumentalPop:	return "Instrumental Pop";
		case InstrumentalRock:	return "Instrumental Rock";
		case Darkwave:			return "Darkwave";
		case TechnoIndustrial:	return "Techno-Industrial";
		case Electronic:		return "Electronic";
		case PopFolk:			return "Pop-Folk";
		case Eurodance:			return "Eurodance";
		case Dream:				return "Dream";
		case SouthernRock:		return "Southern Rock";
		case Comedy:			return "Comedy";
		case Cult:				return "Cult";
		case Gangsta:			return "Gangsta";
		case Top40:				return "Top 40";
		case ChristianRap:		return "Christian Rap";
		case PopOrFunk:			return "Pop/Funk";
		case Jungle:			return "Jungle";
		case NativeAmerican:	return "Native American";
		case Cabaret:			return "Cabaret";
		case NewWave:			return "New Wave";
		case Psychadelic:		return "Psychadelic";
		case Rave:				return "Rave";
		case Showtunes:			return "Showtunes";
		case Trailer:			return "Trailer";
		case LoFi:				return "Lo-Fi";
		case Tribal:			return "Tribal";
		case AcidPunk:			return "Acid Punk";
		case AcidJazz:			return "Acid Jazz";
		case Polka:				return "Polka";
		case Retro:				return "Retro";
		case Musical:			return "Musical";
		case RockAndRoll:		return "Rock & Roll";
		case HardRock:			return "Hard Rock";
		default:				return "";
	}
}

ID3v10::ID3v10(){

}

ID3v10::~ID3v10(){

}

std::string ID3v10::getTitle() const{
	return std::string(reinterpret_cast<const char*>(data.title));
}

void ID3v10::setTitle(std::string& title){
	memset(data.title, 0, 30);
	memcpy(data.title, title.c_str(), title.length());
}

std::string ID3v10::getArtist() const{
	return std::string(reinterpret_cast<const char*>(data.artist));
}

void ID3v10::setArtist(std::string& artist){
	memset(data.artist, 0, 30);
	memcpy(data.artist, artist.c_str(), artist.length());
}

std::string ID3v10::getAlbum() const{
	return std::string(reinterpret_cast<const char*>(data.album));
}

void ID3v10::setAlbum(std::string& album){
	memset(data.album, 0, 30);
	memcpy(data.album, album.c_str(), album.length());
}

std::string ID3v10::getYear() const{
	std::string year;
	year.push_back(data.year[0]);
	year.push_back(data.year[1]);
	year.push_back(data.year[2]);
	year.push_back(data.year[3]);
	return year;
}

void ID3v10::setYear(std::string& year){
	memset(data.year, 0, 4);
	if(year.length() == 4){
		data.year[0] = year[0];
		data.year[1] = year[1];
		data.year[2] = year[2];
		data.year[3] = year[3];
	}
}

std::string ID3v10::getComment() const{
	return reinterpret_cast<const char*>(data.comment);
}

void ID3v10::setComment(std::string& comment){
	memset(data.comment, 0, 30);
	memcpy(data.comment, comment.c_str(), comment.length());
}

ID3v10::Genre ID3v10::getGenre() const{
	return static_cast<ID3v10::Genre>(data.genre[0]);
}

void ID3v10::setGenre(ID3v10::Genre genre){
	data.genre[0] = static_cast<uint8_t>(genre);
}
