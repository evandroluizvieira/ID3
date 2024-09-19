#ifndef ID3V20_HPP
#define ID3V20_HPP

/**
 * @file ID3v20.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 06/07/2024
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the class ID3v20 and class ID3v20Header.
 */

#include "ID3v10.hpp"
#include "ID3v2.hpp"

#include <string>
#include <vector>

/**
 * @class ID3v20Header
 *
 * @brief ID3 tag version 2.0 header derived from ID3v2HeaderBase.
 */
class ID3v20Header : public ID3v2HeaderBase{
	public:
		/**
		 * @brief Default constructor initializing the ID3v2HeaderBase and set major version to 2.
		 */
		ID3v20Header();

		/**
		 * @brief Default virtual destructor that calls ID3v2HeaderBase::~ID3v2HeaderBase().
		 */
		virtual ~ID3v20Header();

		/**
		 * @brief Check if compression flag is set.
		 *
		 * @return true if the compression flag (bit 6 of flags) is set, false otherwise.
		 */
		bool isCompressed() const;

		/**
		 * @brief Set the compression flag.
		 *
		 * @param useCompression When true, sets the compression flag (bit 6 of flags); when false, clears it.
		 */
		void setCompressed(bool useCompression);
};

/**
 * @class ID3v20FrameHeader
 *
 * @brief Represents the header structure for an ID3v2.0 frame header that contains 3 bytes identifier of the frame and 3 bytes size of the frame.
 */
class ID3v20FrameHeader{
	public:
		/**
		 * @brief Default constructor that initializes the object with clear data.
		 */
		ID3v20FrameHeader();

		/**
		 * @brief Default virtual destructor.
		 */
		virtual ~ID3v20FrameHeader();

		/**
		 * @brief Retrieves the size of the frame excluding the header.
		 *
		 * @return The size of the frame.
		 */
		uint32_t getFrameSize() const;

		/**
		 * @brief Sets the size of the frame excluding the header.
		 *
		 * @param size The new size of the frame.
		 *
		 * @note The size must fit within the three-byte limit (up to 0xFFFFFF).
		 */
		void setFrameSize(uint32_t size);

	public:
		/**
		 * @var uint8_t ID3v2FrameHeader::identifier[3]
		 *
		 * @brief 3 character identifier for the frame type.
		 */
		uint8_t identifier[3];

		/**
		 * @var uint8_t ID3v2FrameHeader::size[3]
		 *
		 * @brief 3 byte size descriptor for the frame.
		 */
		uint8_t size[3];
};

/**
 * @struct ID3v20Frame
 *
 * @brief Structure representing an ID3v2.0 frame, consisting of a header and data.
 */
struct ID3v20Frame{
	/**
	 * @brief Default constructor that initializes the object with clear data.
	 */
	ID3v20Frame();

	/**
	 * @brief Default virtual destructor that releases ID3v20Frame::data if has any and clear ID3v20Frame::header.
	 */
	virtual ~ID3v20Frame();

	/**
	 * @var ID3v20FrameHeader ID3v20Frame::header
	 *
	 * @brief Header of the ID3v2.0 frame.
	 */
	ID3v20FrameHeader header;

	/**
	 * @var uint8_t* ID3v20Frame::data
	 *
	 * @brief Pointer to the data of the ID3v2.0 frame.
	 *
	 * @note This pointer may point to dynamically allocated memory for frame data.
	 */
	uint8_t* data;
};

/**
 * @class ID3v20
 *
 * @brief ID3 tag version 2.0 class that holds information in ID3v20Header class.
 *
 * @note If the file has a header version 2.0, it must have at least 1 frame.
 * @note Functions for getting and setting specific frames are based on the limited ID3v1.1. However ID3v20 can include many more types of frames.
 */
class ID3v20{
	public:
		/**
		 * @brief Default constructor.
		 */
		ID3v20();

		/**
		 * @brief Default virtual destructor.
		 */
		virtual ~ID3v20();


		/**
		 * @brief Search through the vector of frames to find a frame with the given identifier.
		 *
		 * @param identifier A 3 byte array representing the identifier of the frame.
		 *
		 * @return A pointer to the ID3v20Frame if found, otherwise nullptr.
		 */
		ID3v20Frame* getFrame(uint8_t identifier[3]) const;

		/**
		 * @brief Updates an existing frame identified by its identifier with new data, or creates a new frame if none with the same identifier exists.
		 *
		 * @param identifier A 3 byte array representing the identifier of the frame.
		 * @param size The size of the data to be set in the frame, in bytes.
		 * @param data Pointer to the data to be copied into the frame.
		 */
		void setFrame(uint8_t identifier[3], uint32_t size, uint8_t* data);

		/**
		 * @brief Removes the first frame with the specified identifier from the tag.
		 *
		 * @param identifier A 3 byte array representing the identifier of the frame.
		 */
		void removeFrame(uint8_t identifier[3]);

		/**
		 * @brief Retrieves the title from the tag.
		 *
		 * @return Copy of the title string if found, otherwise an empty string.
		 */
		std::string getTitle() const;

		/**
		 * @brief Sets the title in the tag.
		 *
		 * @param title The new title to be set.
		 */
		void setTitle(const std::string& title);

		/**
		 * @brief Retrieves the artist from the tag.
		 *
		 * @return Copy of the artist string if found, otherwise an empty string.
		 */
		std::string getArtist() const;

		/**
		 * @brief Sets the artist in the tag.
		 *
		 * @param artist The new artist to be set.
		 */
		void setArtist(const std::string& artist);

		/**
		 * @brief Retrieves the album from the tag.
		 *
		 * @return Copy of the album string if found, otherwise an empty string.
		 */
		std::string getAlbum() const;

		/**
		 * @brief Sets the album in the tag.
		 *
		 * @param album The new album to be set.
		 */
		void setAlbum(const std::string& album);

		/**
		 * @brief Retrieves the year from the tag.
		 *
		 * @return Copy of the year (fixed 4 characters) if found, otherwise an empty string.
		 */
		std::string getYear() const;

		/**
		 * @brief Sets the year in the tag.
		 *
		 * @param year The new year to be set (fixed 4 characters).
		 */
		void setYear(const std::string & year);

		/**
		 * @brief Retrieves the comment from the tag.
		 *
		 * @return Copy of the comment string if found, otherwise an empty string.
		 */
		std::string getComment() const;

		/**
		 * @brief Sets the comment in the tag.
		 *
		 * @param comment The new comment to be set.
		 */
		void setComment(const std::string& comment);

		/**
		 * @brief Retrieves the track number from the tag.
		 *
		 * @return The track number if found, otherwise 0.
		 */
		uint8_t getTrack() const;

		/**
		 * @brief Sets the track number in the tag.
		 *
		 * @param track The new track number to be set.
		 */
		void setTrack(uint8_t track);

		/**
		 * @brief Retrieves the genre from the tag.
		 *
		 * @return The genre as ID3v10::Genre enum if found, otherwise ID3v10::Genre::Other.
		 */
		ID3v10::Genre getGenre() const;

		/**
		 * @brief Sets the genre in the ID3v2.0 tag.
		 *
		 * @param genre The new genre to be set as ID3v10::Genre enum.
		 */
		void setGenre(ID3v10::Genre genre);

	public:
		/**
		 * @var ID3v20Header ID3v20::header
		 *
		 * @brief Class that holds ID3 version 2.0 fields of header data.
		 */
		ID3v20Header header;

		/**
		 * @var std::vector<ID3v20Frame*> ID3v20::frames
		 *
		 * @brief Vector containing pointers to ID3v2.0 frames.
		 *
		 * @note Frames are dynamically allocated and managed.
		 */
		std::vector<ID3v20Frame*> frames;
};

#endif /* ID3V20_HPP */
