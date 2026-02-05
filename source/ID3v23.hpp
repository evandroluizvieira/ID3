#ifndef ID3V23_HPP
#define ID3V23_HPP

/**
 * @file ID3v23.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 06/07/2024
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the class ID3v23 and class ID3v23Header.
 */

#include "ID3v10.hpp"
#include "ID3v2.hpp"

#include <string>
#include <vector>

/**
 * @class ID3v23Header
 *
 * @brief ID3 tag version 2.3 header derived from ID3v2HeaderBase.
 */
class ID3v23Header : public ID3v2HeaderBase{
	public:
		/**
		 * @brief Default constructor initializing the ID3v2HeaderBase and set major version to 3.
		 */
		ID3v23Header();

		/**
		 * @brief Default virtual destructor that calls ID3v2HeaderBase::~ID3v2HeaderBase().
		 */
		virtual ~ID3v23Header();

		/**
		 * @brief Set the extended header flag.
		 *
		 * @param hasExtendedHeader Whether to set or clear the extended header flag.
		 */
		void setExtendedHeader(bool hasExtendedHeader);

		/**
		 * @brief Check if extended header flag is set.
		 *
		 * @return true if the extended header flag is set, false otherwise.
		 */
		bool hasExtendedHeader() const;

		/**
		 * @brief Set the experimental indicator flag.
		 *
		 * @param isExperimental Whether to set or clear the experimental indicator flag.
		 */
		void setExperimental(bool isExperimental);

		/**
		 * @brief Check if experimental indicator flag is set.
		 *
		 * @return true if the experimental indicator flag is set, false otherwise.
		 */
		bool isExperimental() const;
};

/**
 * @class ID3v23ExtendedHeader
 *
 * @brief Represents the ID3v2.3 extended header.
 */
class ID3v23ExtendedHeader{
	public:
		/**
		 * @brief Default constructor initializing the extended header.
		 */
		ID3v23ExtendedHeader();

		/**
		 * @brief Destructor for the extended header.
		 */
		~ID3v23ExtendedHeader();

		/**
		 * @brief Check if the CRC flag is set.
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool hasCRC() const;

		/**
		 * @brief Set the CRC-32 data.
		 *
		 * @param value The CRC-32 value to set.
		 */
		void setCRC(uint32_t crcValue);

		/**
		 * @brief Get the CRC-32 data.
		 *
		 * @return The CRC-32 value, or 0 if not present.
		 */
		uint32_t getCRC() const;

	    /**
	     * @brief Remove the CRC-32 data, clearing the CRC flag and deallocates the memory for CRC, if it exists.
	     */
	    void removeCRC();

		/**
		 * @brief Get the total size of the extended header.
		 *
		 * @return The size of the extended header, including CRC if present.
		 */
		uint32_t getSize() const;

	public:
		uint32_t size;
		uint16_t flags;
		uint32_t padding;
		uint32_t* crc;
};

/**
 * @class ID3v23FrameHeader
 *
 * @brief Represents the header structure for an ID3v2.3 frame header that contains 4 bytes identifier of the frame, 4 bytes size of the frame, and 2 bytes flags.
 */
class ID3v23FrameHeader{
	public:
		/**
		 * @brief Default constructor that initializes the object with clear data.
		 */
		ID3v23FrameHeader();

		/**
		 * @brief Default virtual destructor.
		 */
		virtual ~ID3v23FrameHeader();

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
		 */
		void setFrameSize(uint32_t size);

		/**
		 * @brief Set the tag alter preservation flag.
		 *
		 * @param preserve Whether to set or clear the flag.
		 */
		void setTagAlterPreservation(bool preserve);

		/**
		 * @brief Check if the tag alter preservation flag is set.
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool getTagAlterPreservation() const;

		/**
		 * @brief Set the file alter preservation flag.
		 *
		 * @param preserve Whether to set or clear the flag.
		 */
		void setFileAlterPreservation(bool preserve);

		/**
		 * @brief Check if the file alter preservation flag is set.
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool getFileAlterPreservation() const;

		/**
		 * @brief Set the read-only flag.
		 *
		 * @param readOnly Whether to set or clear the flag.
		 */
		void setReadOnly(bool readOnly);

		/**
		 * @brief Check if the read-only flag is set.
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool isReadOnly() const;

		/**
		 * @brief Set the compression flag.
		 *
		 * @param compressed Whether to set or clear the flag.
		 */
		void setCompressed(bool compressed);

		/**
		 * @brief Check if the compression flag is set.
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool isCompressed() const;

		/**
		 * @brief Set the encryption flag.
		 *
		 * @param encrypted Whether to set or clear the flag.
		 */
		void setEncrypted(bool encrypted);

		/**
		 * @brief Check if the encryption flag is set.
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool isEncrypted() const;

		/**
		 * @brief Set the grouping identity flag.
		 *
		 * @param group Whether to set or clear the flag.
		 */
		void setGroupingIdentity(bool group);

		/**
		 * @brief Check if the grouping identity flag is set.
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool isGroupingIdentity() const;

	public:
		/**
		 * @var uint8_t ID3v23FrameHeader::identifier[4]
		 *
		 * @brief 4 character identifier for the frame type.
		 */
		uint8_t identifier[4];

		/**
		 * @var uint8_t ID3v23FrameHeader::size[4]
		 *
		 * @brief 4 byte size descriptor for the frame.
		 */
		uint8_t size[4];

		/**
		 * @var uint8_t ID3v23FrameHeader::flags[2]
		 *
		 * @brief 2 byte flags for the frame.
		 */
		uint8_t flags[2];
};

/**
 * @struct ID3v23Frame
 *
 * @brief Structure representing an ID3v2.3 frame, consisting of a header and data.
 */
struct ID3v23Frame {
    /**
     * @brief Default constructor that initializes the object with clear data.
     */
    ID3v23Frame();

    /**
     * @brief Default virtual destructor that releases ID3v23Frame::data if has any and clear ID3v23Frame::header.
     */
    virtual ~ID3v23Frame();

    /**
     * @var ID3v23FrameHeader ID3v23Frame::header
     *
     * @brief Header of the ID3v2.3 frame.
     */
    ID3v23FrameHeader header;

    /**
     * @var uint8_t* ID3v23Frame::data
     *
     * @brief Pointer to the data of the ID3v2.3 frame.
     *
     * @note This pointer may point to dynamically allocated memory for frame data.
     */
    uint8_t* data;
};

/**
 * @class ID3v23
 *
 * @brief ID3 tag version 2.3 class that holds information in ID3v23Header class.
 */
class ID3v23{
	public:
		/**
		 * @brief Default constructor.
		 */
		ID3v23();

		/**
		 * @brief Default virtual destructor.
		 */
		virtual ~ID3v23();

		void print();

		/**
		 * @brief Search through the vector of frames to find a frame with the given identifier.
		 *
		 * @param identifier A 4 byte array representing the identifier of the frame.
		 *
		 * @return A pointer to the first ID3v23Frame if found, otherwise nullptr.
		 */
		ID3v23Frame* getFrame(uint8_t identifier[4]) const;

		/**
		 * @brief Updates the first occurrence of a frame with the given identifier, if present.
		 *
		 * If a frame with the given identifier exists, its data is replaced and returns true. If not, does nothing and returns false.
		 *
		 * @param identifier A 4 byte array representing the identifier of the frame.
		 * @param size The size of the data to be set in the frame, in bytes.
		 * @param data Pointer to the data to be copied into the frame.
		 * @return true if a frame was found and updated, false otherwise.
		 */
		bool setFrame(uint8_t identifier[4], uint32_t size, uint8_t* data);

		/**
		 * @brief Adds a new frame with the given identifier and data, regardless of existing frames.
		 *
		 * This always appends a new frame to the tag, even if other frames with the same identifier exist.
		 *
		 * @param identifier A 4 byte array representing the identifier of the frame.
		 * @param size The size of the data to be set in the frame, in bytes.
		 * @param data Pointer to the data to be copied into the frame.
		 */
		void addFrame(uint8_t identifier[4], uint32_t size, uint8_t* data);

		/**
		 * @brief Removes a specified frame from the tag.
		 *
		 * @param frame Pointer to the ID3v23Frame object to be removed from the tag.
		 */
		void removeFrame(ID3v23Frame* frame);

		/**
		 * @brief Retrieves the title from the tag.
		 *
		 * @return Copy of the title string if found, otherwise an empty string.
		 */
		std::string getTitle() const;

		/**
		 * @brief Sets the title in the tag.
		 *
		 * @param title The title to set.
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
		virtual std::string getYear() const;

		/**
		* @brief Sets the year in the tag.
		*
		* @param year The new year to be set (fixed 4 characters).
		*/
		virtual void setYear(const std::string& year);

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
		std::string getTrack() const;

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
		std::string getGenre() const;

		/**
		 * @brief Sets the genre in the ID3v2.0 tag.
		 *
		 * @param genre The new genre to be set as ID3v10::Genre enum.
		 */
		void setGenre(ID3v10::Genre genre);

	public:
		/**
		 * @var ID3v23Header ID3v23::header
		 *
		 * @brief Class that holds ID3 version 2.3 fields of header data.
		 */
		ID3v23Header header;

	    /**
	     * @var ID3v23ExtendedHeader* ID3v23::extendedHeader
	     *
	     * @brief Pointer to the extended header, if present. This can be null if the tag does not have an extended header.
	     *
	     * @note The extended header is dynamically allocated and managed.
	     */
		ID3v23ExtendedHeader* extendedHeader;

		/**
		 * @var std::vector<ID3v23Frame*> ID3v23::frames
		 *
		 * @brief Vector containing pointers to ID3v2.3 frames.
		 *
		 * @note Frames are dynamically allocated and managed.
		 */
		std::vector<ID3v23Frame*> frames;
};

#endif /* ID3V23_HPP */
