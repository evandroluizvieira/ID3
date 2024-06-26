#ifndef ID3V10_HPP
#define ID3V10_HPP

#include <cstdint>
#include <string>

/*
 * @brief: ID3 version 1.0 data structure.
 */
struct ID3v10Data{
    uint8_t identification[3];
    uint8_t title[30];
    uint8_t artist[30];
    uint8_t album[30];
    uint8_t year[4];
    uint8_t comment[30];
    uint8_t genre[1];
};

/*
 * @brief: ID3 tag version 1.0 that holds information in 'data'.
 */
class ID3v10{
    public:

		/*
		 * @brief: List of all default music genres.
		 */
        enum Genre{
            Blues = 0,
            ClassicRock = 1,
            Country = 2,
            Dance = 3,
            Disco = 4,
            Funk = 5,
            Grunge = 6,
            HipHop = 7,
            Jazz = 8,
            Metal = 9,
            NewAge = 10,
            Oldies = 11,
            Other = 12,
            Pop = 13,
            RAndB = 14,
            Rap = 15,
            Reggae = 16,
            Rock = 17,
            Techno = 18,
            Industrial = 19,
            Alternative = 20,
            Ska = 21,
            DeathMetal = 22,
            Pranks = 23,
            Soundtrack = 24,
            EuroTechno = 25,
            Ambient = 26,
            TripHop = 27,
            Vocal = 28,
            JazzFunk = 29,
            Fusion = 30,
            Trance = 31,
            Classical = 32,
            Instrumental = 33,
            Acid = 34,
            House = 35,
            Game = 36,
            SoundClip = 37,
            Gospel = 38,
            Noise = 39,
            AlternRock = 40,
            Bass = 41,
            Soul = 42,
            Punk = 43,
            Space = 44,
            Meditative = 45,
            InstrumentalPop = 46,
            InstrumentalRock = 47,
            Ethnic = 48,
            Gothic = 49,
            Darkwave = 50,
            TechnoIndustrial = 51,
            Electronic = 52,
            PopFolk = 53,
            Eurodance = 54,
            Dream = 55,
            SouthernRock = 56,
            Comedy = 57,
            Cult = 58,
            Gangsta = 59,
            Top40 = 60,
            ChristianRap = 61,
            PopOrFunk = 62,
            Jungle = 63,
            NativeAmerican = 64,
            Cabaret = 65,
            NewWave = 66,
            Psychadelic = 67,
            Rave = 68,
            Showtunes = 69,
            Trailer = 70,
            LoFi = 71,
            Tribal = 72,
            AcidPunk = 73,
            AcidJazz = 74,
            Polka = 75,
            Retro = 76,
            Musical = 77,
            RockAndRoll = 78,
            HardRock = 79
        };

    public:
        /*
		 * @brief: Default constructor.
		 */
        ID3v10();

        /*
		 * @brief: Default virtual destructor.
		 */
        virtual ~ID3v10();

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
		 * @brief: Get 'data.genre' as std::string.
		 * @return: Copy of the data.
		 */
        ID3v10::Genre getGenre() const;

        /*
		 * @brief: Set a new genre in 'data.genre'.
		 */
        void setGenre(ID3v10::Genre genre);

    public:
        ID3v10Data data;
};

#endif /* ID3V10_HPP */
