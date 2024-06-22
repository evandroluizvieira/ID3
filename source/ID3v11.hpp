#ifndef ID3V11_HPP
#define ID3V11_HPP

#include "ID3v10.hpp"

#include <cstdint>
#include <string>

/*
 * @brief: ID3 version 1.1 data structure.
 */
struct ID3v11Data{
    uint8_t identification[3];
    uint8_t title[30];
    uint8_t artist[30];
    uint8_t album[30];
    uint8_t year[4];
    uint8_t comment[29];
    uint8_t track[1];
    uint8_t genre[1];
};

/*
 * @brief: ID3 tag version 1.1 that holds information in 'data'.
 */
class ID3v11{
    public:
		/*
		 * @brief: Default constructor.
		 */
        ID3v11();

        /*
		 * @brief: Default virtual destructor.
		 */
        virtual ~ID3v11();

    public:
        /*
		 * @brief: Get 'data.title' as std::string.
		 * @return: Copy of the data.
		 */
        std::string getTitle() const;

        /*
		 * @brief: Set a new title in 'data.title'.
		 */
		void setTitle(std::string& title);

		/*
		 * @brief: Get 'data.artist' as std::string.
		 * @return: Copy of the data.
		 */
		std::string getArtist() const;

		/*
		 * @brief: Set a new artist in 'data.artist'.
		 */
		void setArtist(std::string& artist);

		/*
		 * @brief: Get 'data.album' as std::string.
		 * @return: Copy of the data.
		 */
		std::string getAlbum() const;

		/*
		 * @brief: Set a new album in 'data.album'.
		 */
		void setAlbum(std::string& album);

		/*
		 * @brief: Get 'data.year' as std::string.
		 * @return: Copy of the data.
		 */
		std::string getYear() const;

		/*
		 * @brief: Set a new year in 'data.year'.
		 */
		void setYear(std::string& year);

		/*
		 * @brief: Get 'data.comment' as std::string.
		 * @return: Copy of the data.
		 */
		std::string getComment() const;

		/*
		 * @brief: Set a new comment in 'data.comment'.
		 */
		void setComment(std::string& comment);

		/*
		 * @brief: Get 'data.track' as int.
		 * @return: Copy of the data.
		 */
        int getTrack() const;

        /*
		 * @brief: Set a new track in 'data.track'.
		 */
        void setTrack(int track);

        /*
		 * @brief: Get 'data.genre' as std::string.
		 * @return: Copy of the data.
		 */
        ID3v10::Genre getGenre() const;

        /*
		 * @brief: Set a new genre in 'data.genre'.
		 */
        void setGenre(ID3v10::Genre genre);

    public:
        ID3v11Data data;
};

#endif /* ID3V11_HPP */
