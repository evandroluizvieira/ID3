#ifndef ID3V10_HPP
#define ID3V10_HPP

/**
 * @file ID3v10.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 06/07/2024
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the class ID3v10 and struct ID3v10Data.
 */

#include <cstdint>
#include <string>

/**
 * @struct ID3v10Data
 *
 * @brief ID3 tag version 1.0 data structure.
 */
struct ID3v10Data{
	/**
	 * @var uint8_t ID3v10Data::identification[3]
	 *
	 * @brief Array holding 3-byte text identification for the tag.
	 */
	uint8_t identification[3];

	/**
	 * @var uint8_t ID3v10Data::title[30]
	 *
	 * @brief Array holding 30-byte title text information of the tag.
	 */
	uint8_t title[30];

	/**
	 * @var uint8_t ID3v10Data::artist[30]
	 *
	 * @brief Array holding 30-byte artist text information of the tag.
	 */
	uint8_t artist[30];

	/**
	 * @var uint8_t ID3v10Data::album[30]
	 *
	 * @brief Array holding 30-byte album text information of the tag.
	 */
	uint8_t album[30];

	/**
	 * @var uint8_t ID3v10Data::year[4]
	 *
	 * @brief Array holding 4-byte year text information of the tag.
	 */
	uint8_t year[4];

	/**
	 * @var uint8_t ID3v10Data::comment[30]
	 *
	 * @brief Array holding 30-byte comment text information of the tag.
	 */
	uint8_t comment[30];

	/**
	 * @var uint8_t ID3v10Data::genre[1]
	 *
	 * @brief Array holding 1-byte genre number information of the tag.
	 */
	uint8_t genre[1];
};

/**
 * @class ID3v10
 *
 * @brief ID3 tag version 1.0 class that holds information in ID3v10Data structure, providing methods to access and modify ID3v1.0 tag information.
 */
class ID3v10{
	friend class MP3File;

	public:
		/**
	     * @enum ID3v10::Genre
	     *
	     * @brief List of default music genres for ID3v1.0 tags.
	     */
		enum Genre{
	        Blues = 0,               /**< Blues music genre. */
	        ClassicRock = 1,         /**< Classic Rock music genre. */
	        Country = 2,             /**< Country music genre. */
	        Dance = 3,               /**< Dance music genre. */
	        Disco = 4,               /**< Disco music genre. */
	        Funk = 5,                /**< Funk music genre. */
	        Grunge = 6,              /**< Grunge music genre. */
	        HipHop = 7,              /**< Hip Hop music genre. */
	        Jazz = 8,                /**< Jazz music genre. */
	        Metal = 9,               /**< Metal music genre. */
	        NewAge = 10,             /**< New Age music genre. */
	        Oldies = 11,             /**< Oldies music genre. */
	        Other = 12,              /**< Other music genre. */
	        Pop = 13,                /**< Pop music genre. */
	        RAndB = 14,              /**< R&B music genre. */
	        Rap = 15,                /**< Rap music genre. */
	        Reggae = 16,             /**< Reggae music genre. */
	        Rock = 17,               /**< Rock music genre. */
	        Techno = 18,             /**< Techno music genre. */
	        Industrial = 19,         /**< Industrial music genre. */
	        Alternative = 20,        /**< Alternative music genre. */
	        Ska = 21,                /**< Ska music genre. */
	        DeathMetal = 22,         /**< Death Metal music genre. */
	        Pranks = 23,             /**< Pranks music genre. */
	        Soundtrack = 24,         /**< Soundtrack music genre. */
	        EuroTechno = 25,         /**< Euro Techno music genre. */
	        Ambient = 26,            /**< Ambient music genre. */
	        TripHop = 27,            /**< Trip Hop music genre. */
	        Vocal = 28,              /**< Vocal music genre. */
	        JazzFunk = 29,           /**< Jazz Funk music genre. */
	        Fusion = 30,             /**< Fusion music genre. */
	        Trance = 31,             /**< Trance music genre. */
	        Classical = 32,          /**< Classical music genre. */
	        Instrumental = 33,       /**< Instrumental music genre. */
	        Acid = 34,               /**< Acid music genre. */
	        House = 35,              /**< House music genre. */
	        Game = 36,               /**< Game music genre. */
	        SoundClip = 37,          /**< Sound Clip music genre. */
	        Gospel = 38,             /**< Gospel music genre. */
	        Noise = 39,              /**< Noise music genre. */
	        AlternRock = 40,         /**< Alternative Rock music genre. */
	        Bass = 41,               /**< Bass music genre. */
	        Soul = 42,               /**< Soul music genre. */
	        Punk = 43,               /**< Punk music genre. */
	        Space = 44,              /**< Space music genre. */
	        Meditative = 45,         /**< Meditative music genre. */
	        InstrumentalPop = 46,    /**< Instrumental Pop music genre. */
	        InstrumentalRock = 47,   /**< Instrumental Rock music genre. */
	        Ethnic = 48,             /**< Ethnic music genre. */
	        Gothic = 49,             /**< Gothic music genre. */
	        Darkwave = 50,           /**< Darkwave music genre. */
	        TechnoIndustrial = 51,   /**< Techno Industrial music genre. */
	        Electronic = 52,         /**< Electronic music genre. */
	        PopFolk = 53,            /**< Pop Folk music genre. */
	        Eurodance = 54,          /**< Eurodance music genre. */
	        Dream = 55,              /**< Dream music genre. */
	        SouthernRock = 56,       /**< Southern Rock music genre. */
	        Comedy = 57,             /**< Comedy music genre. */
	        Cult = 58,               /**< Cult music genre. */
	        Gangsta = 59,            /**< Gangsta music genre. */
	        Top40 = 60,              /**< Top 40 music genre. */
	        ChristianRap = 61,       /**< Christian Rap music genre. */
	        PopOrFunk = 62,          /**< Pop or Funk music genre. */
	        Jungle = 63,             /**< Jungle music genre. */
	        NativeAmerican = 64,     /**< Native American music genre. */
	        Cabaret = 65,            /**< Cabaret music genre. */
	        NewWave = 66,            /**< New Wave music genre. */
	        Psychadelic = 67,        /**< Psychadelic music genre. */
	        Rave = 68,               /**< Rave music genre. */
	        Showtunes = 69,          /**< Showtunes music genre. */
	        Trailer = 70,            /**< Trailer music genre. */
	        LoFi = 71,               /**< Lo-Fi music genre. */
	        Tribal = 72,             /**< Tribal music genre. */
	        AcidPunk = 73,           /**< Acid Punk music genre. */
	        AcidJazz = 74,           /**< Acid Jazz music genre. */
	        Polka = 75,              /**< Polka music genre. */
	        Retro = 76,              /**< Retro music genre. */
	        Musical = 77,            /**< Musical music genre. */
	        RockAndRoll = 78,        /**< Rock and Roll music genre. */
	        HardRock = 79            /**< Hard Rock music genre. */
	    };

		/**
		 * @brief Default constructor that initializes the object with clear data.
		 */
		ID3v10();

		/**
		 * @brief Virtual destructor that finalizes the object with clear data.
		 */
		virtual ~ID3v10();

		/**
		 * @brief Get the title from 'ID3v10::data.title' as std::string.
		 *
		 * @return Copy of the title (up to 30 characters).
		 */
		std::string getTitle() const;

		/**
		 * @brief Set a new title in 'ID3v10::data.title'.
		 *
		 * @param title New title to set (up to 30 characters).
		 */
		void setTitle(const std::string& title);

		/**
		 * @brief Get the artist from 'ID3v10::data.artist' as std::string.
		 *
		 * @return Copy of the artist name (up to 30 characters).
		 */
		std::string getArtist() const;

		/**
		 * @brief Set a new artist in 'ID3v10::data.artist'.
		 *
		 * @param artist New artist name to set (up to 30 characters).
		 */
		void setArtist(const std::string& artist);

		/**
		 * @brief Get the album from 'ID3v10::data.album' as std::string.
		 *
		 * @return Copy of the album name (up to 30 characters).
		 */
		std::string getAlbum() const;

		/**
		 * @brief Set a new album in 'ID3v10::data.album'.
		 *
		 * @param album New album name to set (up to 30 characters).
		 */
		void setAlbum(const std::string& album);

		/**
		 * @brief Get the year from 'ID3v10::data.year' as std::string.
		 *
		 * @return Copy of the year (fixed 4 characters).
		 */
		std::string getYear() const;

		/**
		 * @brief Set a new year in 'ID3v10::data.year'.
		 *
		 * @param year New year to set (fixed 4 characters).
		 */
		void setYear(const std::string& year);

		/**
		 * @brief Get the comment from 'ID3v10::data.comment' as std::string.
		 *
		 * @return Copy of the comment (up to 30 characters).
		 */
		std::string getComment() const;

		/**
		 * @brief Set a new comment in 'ID3v10::data.comment'.
		 *
		 * @param comment New comment to set (up to 30 characters).
		 */
		void setComment(const std::string& comment);

		/**
		 * @brief Get the genre from 'ID3v10::data.genre' as ID3v10::Genre.
		 *
		 * @return Genre index.
		 */
		ID3v10::Genre getGenre() const;

		/**
		 * @brief Set a new genre in 'ID3v10::data.genre'.
		 *
		 * @param genre Genre index to set.
		 */
		void setGenre(ID3v10::Genre genre);

    private:
	   /**
	    * @var ID3v10Data ID3v10::data
	    *
	    * @brief Structure holding ID3 version 1.0 data fields.
	    */
	   ID3v10Data data;
};

#endif /* ID3V10_HPP */
