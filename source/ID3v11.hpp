#ifndef ID3V11_HPP
#define ID3V11_HPP

/**
 * @file ID3v11.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 06/07/2024
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the class ID3v11 and struct ID3v11Data.
 */

#include "ID3v10.hpp"

#include <cstdint>
#include <string>

/**
 * @struct ID3v11Data
 *
 * @brief ID3 tag version 1.1 data structure.
 */
struct ID3v11Data{
	/**
	 * @var uint8_t ID3v11Data::identification[3]
	 *
	 * @brief Array holding 3-byte text identification for the tag.
	*/
	uint8_t identification[3];

	/**
	 * @var uint8_t ID3v11Data::title[30]
	 *
	 * @brief Array holding 30-byte title text information of the tag.
	 */
	uint8_t title[30];

	/**
	 * @var uint8_t ID3v11Data::artist[30]
	 *
	 * @brief Array holding 30-byte artist text information of the tag.
	 */
	uint8_t artist[30];

	/**
	 * @var uint8_t ID3v11Data::album[30]
	 *
	 * @brief Array holding 30-byte album text information of the tag.
	 */
	uint8_t album[30];

	/**
	 * @var uint8_t ID3v11Data::year[4]
	 *
	 * @brief Array holding 4-byte year text information of the tag.
	 */
	uint8_t year[4];

	/**
	 * @var uint8_t ID3v11Data::comment[29]
	 *
	 * @brief Array holding 29-byte comment text information of the tag.
	 */
	uint8_t comment[29];

	/**
	 * @var uint8_t ID3v11Data::track[1]
	 *
	 * @brief Array holding 1-byte track number information of the tag.
	 */
	uint8_t track[1];

	/**
	 * @var uint8_t ID3v11Data::genre[1]
	 *
	 * @brief Array holding 1-byte genre number information of the tag.
	 */
	uint8_t genre[1];
};

/**
 * @class ID3v11
 *
 * @brief ID3 tag version 1.1 class that holds information in ID3v11Data structure, providing methods to access and modify ID3v1.1 tag information.
 */
class ID3v11{
	friend class MP3File;

	public:
		/**
		 * @brief Default constructor that initializes the object with clear data.
		 */
		ID3v11();

		/**
		 * @brief Virtual destructor that finalizes the object with clear data.
		 */
		virtual ~ID3v11();

		/**
		 * @brief Get the title from 'ID3v11::data.title' as std::string.
		 *
		 * @return Copy of the title (up to 30 characters).
		 */
		std::string getTitle() const;

		/**
		 * @brief Set a new title in 'ID3v11::data.title'.
		 *
		 * @param title New title to set (up to 30 characters).
		 */
		void setTitle(const std::string& title);

		/**
		 * @brief Get the artist from 'ID3v11::data.artist' as std::string.
		 *
		 * @return Copy of the artist name (up to 30 characters).
		 */
		std::string getArtist() const;

		/**
		 * @brief Set a new artist in 'ID3v11::data.artist'.
		 *
		 * @param artist New artist name to set (up to 30 characters).
		 */
		void setArtist(const std::string& artist);

		/**
		 * @brief Get the album from 'ID3v11::data.album' as std::string.
		 *
		 * @return Copy of the album name (up to 30 characters).
		 */
		std::string getAlbum() const;

		/**
		 * @brief Set a new album in 'ID3v11::data.album'.
		 *
		 * @param album New album name to set (up to 30 characters).
		 */
		void setAlbum(const std::string& album);

		/**
		 * @brief Get the year from 'ID3v11::data.year' as std::string.
		 *
		 * @return Copy of the year (fixed 4 characters).
		 */
		std::string getYear() const;

		/**
		 * @brief Set a new year in 'ID3v11::data.year'.
		 *
		 * @param year New year to set (fixed 4 characters).
		 */
		void setYear(const std::string& year);

		/**
		 * @brief Get the comment from 'ID3v11::data.comment' as std::string.
		 *
		 * @return Copy of the comment (up to 29 characters).
		 */
		std::string getComment() const;

		/**
		 * @brief Set a new comment in 'ID3v11::data.comment'.
		 *
		 * @param comment New comment to set (up to 29 characters).
		 */
		void setComment(const std::string& comment);

		/**
		 * @brief Get the track number from 'ID3v11::data.track' as unsigned int.
		 *
		 * @return Track number.
		 */
		uint8_t getTrack() const;

		/**
		 * @brief Set a new track number in 'ID3v11::data.track'.
		 *
		 * @param track New track number to set.
		 */
		void setTrack(uint8_t track);

		/**
		 * @brief Get the genre from 'ID3v11::data.genre' as ID3v10::Genre.
		 *
		 * @return Genre index.
		 */
		ID3v10::Genre getGenre() const;

		/**
		 * @brief Set a new genre in 'ID3v11::data.genre'.
		 *
		 * @param genre Genre index to set.
		 */
		void setGenre(ID3v10::Genre genre);

	private:
		/**
		 * @var ID3v11Data ID3v11::data
		 *
		 * @brief Structure holding ID3 version 1.1 data fields.
		 */
		ID3v11Data data;
};

#endif /* ID3V11_HPP */
