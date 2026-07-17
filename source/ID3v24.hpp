#ifndef ID3V24_HPP
#define ID3V24_HPP

/**
 * @file ID3v24.hpp
 *
 * @author Evandro L. Vieira
 *
 * @brief This file contains the definition of the class ID3v24 and related classes.
 */

#include "ID3v2.hpp"

#include <string>
#include <vector>

/**
 * @class ID3v24Header
 *
 * @brief ID3 tag version 2.4 header derived from ID3v2HeaderBase.
 */
class ID3v24Header : public ID3v2HeaderBase{
public:
    /**
     * @brief Default constructor initializing the ID3v2HeaderBase and set major version to 4.
     */
    ID3v24Header();

    /**
     * @brief Default virtual destructor that calls ID3v2HeaderBase::~ID3v2HeaderBase().
     */
    virtual ~ID3v24Header();

    /**
     * @brief Set the footer flag.
     *
     * @param hasFooter Whether to set or clear the footer flag.
     */
    void setFooter(bool hasFooter);

    /**
     * @brief Check if footer present flag is set.
     *
     * @return true if the footer present flag is set, false otherwise.
     */
    bool hasFooter() const;
};

/**
 * @class ID3v24ExtendedHeader
 *
 * @brief Represents the ID3v2.4 extended header.
 */
class ID3v24ExtendedHeader{
	public:
		/**
		 * @brief Default constructor initializing the extended header.
		 */
		ID3v24ExtendedHeader();

		/**
		 * @brief Destructor for the extended header.
		 */
		~ID3v24ExtendedHeader();

		/**
		 * @brief Check if the CRC flag is set (bit 5 of extFlags).
		 *
		 * @return true if the flag is set, false otherwise.
		 */
		bool hasCRC() const;

		/**
		 * @brief Set the CRC-32 data and enable the CRC flag.
		 *
		 * @param crcValue The CRC-32 value to set (stored as plain integer).
		 */
		void setCRC(uint32_t crcValue);

		/**
		 * @brief Get the CRC-32 data.
		 *
		 * @return The CRC-32 value, or 0 if not present.
		 */
		uint32_t getCRC() const;

		/**
		 * @brief Remove the CRC-32 data, clearing the CRC flag.
		 */
		void removeCRC();

		/**
		 * @brief Check if the update flag is set (bit 6 of extFlags).
		 *
		 * @return true if the update flag is set, false otherwise.
		 */
		bool isUpdate() const;

		/**
		 * @brief Set the update flag.
		 *
		 * @param update Whether to set or clear the update flag.
		 */
		void setUpdate(bool update);

		/**
		 * @brief Get the total size of the extended header (including the 4 size bytes).
		 *
		 * @return The total size in bytes.
		 */
		uint32_t getSize() const;

	public:
        /**
		 * @var uint32_t ID3v24ExtendedHeader::size
		 *
		 * @brief Size of the rest of the extended header excluding the 4 size bytes (plain integer).
		 */
		uint32_t size;

		/**
		 * @var uint8_t ID3v24ExtendedHeader::flagBytes
		 *
		 * @brief Number of flag bytes. MUST be 1 for ID3v2.4.
		 */
		uint8_t flagBytes;

		/**
		 * @var uint8_t ID3v24ExtendedHeader::flags
		 *
		 * @brief Flags byte: bit6=update, bit5=CRC, bit4=restrictions.
		 */
		uint8_t flags;

		/**
		 * @var uint32_t* ID3v24ExtendedHeader::crc
		 *
		 * @brief Pointer to CRC-32 data (4 bytes). nullptr if not present.
		 */
		uint32_t* crc;
};

/**
 * @class ID3v24FrameHeader
 *
 * @brief Represents the header structure for an ID3v2.4 frame.
 *        Inherits common fields and implements version‑specific methods.
 *
 * @see ID3v2FrameHeaderBase
 */
class ID3v24FrameHeader : public ID3v2FrameHeaderBase{
public:
    /**
     * @brief Default constructor that initializes the object with clear data.
     */
    ID3v24FrameHeader();

    /**
     * @brief Default virtual destructor.
     */
    virtual ~ID3v24FrameHeader();

    /**
     * @brief Retrieves the size of the frame excluding the header (synchsafe).
     *
     * @return The size of the frame (decoded from synchsafe).
     */
    virtual uint32_t getFrameSize() const override;

    /**
     * @brief Sets the size of the frame excluding the header (synchsafe).
     *
     * @param size The new size of the frame.
     */
    virtual void setFrameSize(uint32_t size) override;

    /**
     * @brief Set the data length indicator flag (bit 0 of flags[1]).
     *
     * @param hasIndicator Whether to set or clear the flag.
     */
    void setDataLengthIndicator(bool hasIndicator);

    /**
     * @brief Check if the data length indicator flag is set.
     *
     * @return true if the flag is set, false otherwise.
     */
    bool hasDataLengthIndicator() const;

    /**
     * @brief Set the unsynchronisation flag (bit 1 of flags[1]).
     *
     * @param unsync Whether to set or clear the flag.
     */
    void setUnsynchronised(bool unsync);

    /**
     * @brief Check if the unsynchronisation flag is set.
     *
     * @return true if the flag is set, false otherwise.
     */
    bool isUnsynchronised() const;

    /**
     * @brief Set the encryption flag (bit 2 of flags[1]).
     *
     * @param encrypted Whether to set or clear the flag.
     */
    virtual void setEncrypted(bool encrypted) override;

    /**
     * @brief Check if the encryption flag is set.
     *
     * @return true if the flag is set, false otherwise.
     */
    virtual bool isEncrypted() const override;

    /**
     * @brief Set the compression flag (bit 3 of flags[1]).
     *
     * @param compressed Whether to set or clear the flag.
     */
    virtual void setCompressed(bool compressed) override;

    /**
     * @brief Check if the compression flag is set.
     *
     * @return true if the flag is set, false otherwise.
     */
    virtual bool isCompressed() const override;

    /**
     * @brief Set the read‑only flag (bit 4 of flags[0]).
     *
     * @param readOnly Whether to set or clear the flag.
     */
    virtual void setReadOnly(bool readOnly) override;

    /**
     * @brief Check if the read‑only flag is set.
     *
     * @return true if the flag is set, false otherwise.
     */
    virtual bool isReadOnly() const override;

    /**
     * @brief Set the grouping identity flag (bit 6 of flags[1]).
     *
     * @param group Whether to set or clear the flag.
     */
    virtual void setGroupingIdentity(bool group) override;

    /**
     * @brief Check if the grouping identity flag is set.
     *
     * @return true if the flag is set, false otherwise.
     */
    virtual bool isGroupingIdentity() const override;
};

/**
 * @struct ID3v24Frame
 *
 * @brief Structure representing an ID3v2.4 frame, consisting of a header and data.
 */
struct ID3v24Frame {
    /**
     * @brief Default constructor that initializes the object with clear data.
     */
    ID3v24Frame();

    /**
     * @brief Default virtual destructor that releases data and clears header.
     */
    virtual ~ID3v24Frame();

    /**
     * @var ID3v24FrameHeader ID3v24Frame::header
     *
     * @brief Header of the ID3v2.4 frame.
     */
    ID3v24FrameHeader header;

    /**
     * @var uint8_t* ID3v24Frame::data
     *
     * @brief Pointer to the data of the ID3v2.4 frame.
     *
     * @note This pointer may point to dynamically allocated memory for frame data.
     */
    uint8_t* data;
};

/**
 * @class ID3v24Footer
 *
 * @brief Optional footer for ID3v2.4 tags (10 bytes).
 */
class ID3v24Footer{
public:
    /**
     * @brief Default constructor initializing footer with "3DI" and version 4.
     */
    ID3v24Footer();

    /**
     * @brief Destructor.
     */
    ~ID3v24Footer();

    /**
     * @brief Set the footer flags (should match the header flags).
     *
     * @param flagsValue The flags byte.
     */
    void setFlags(uint8_t flagsValue);

    /**
     * @brief Get the footer flags.
     *
     * @return The flags byte.
     */
    uint8_t getFlags() const;

    /**
     * @brief Set the tag size (synchsafe) for the footer.
     *
     * @param tagSize The total tag size (excluding header/footer).
     */
    void setSize(uint32_t tagSize);

    /**
     * @brief Get the tag size (decoded from synchsafe).
     *
     * @return The tag size.
     */
    uint32_t getSize() const;

public:
    /**
     * @var uint8_t ID3v24Footer::identifier[3]
     *
     * @brief Always "3DI".
     */
    uint8_t identifier[3];

    /**
     * @var uint8_t ID3v24Footer::major_version
     *
     * @brief Major version (4).
     */
    uint8_t major_version;

    /**
     * @var uint8_t ID3v24Footer::revision
     *
     * @brief Revision (0).
     */
    uint8_t revision;

    /**
     * @var uint8_t ID3v24Footer::flags
     *
     * @brief Same flags as header (bit4 = footer present).
     */
    uint8_t flags;

    /**
     * @var uint8_t ID3v24Footer::size[4]
     *
     * @brief Tag size encoded as synchsafe integer (4 bytes).
     */
    uint8_t size[4];
};

/**
 * @class ID3v24
 *
 * @brief ID3 tag version 2.4 class that holds header, optional extended header,
 *        optional footer, and a list of frames.
 */
class ID3v24{
public:
    /**
     * @brief Default constructor.
     */
    ID3v24();

    /**
     * @brief Default virtual destructor.
     */
    virtual ~ID3v24();

    /**
     * @brief Search through the vector of frames to find a frame with the given identifier.
     *
     * @param identifier A 4 byte array representing the identifier of the frame.
     *
     * @return A pointer to the first ID3v24Frame if found, otherwise nullptr.
     */
    ID3v24Frame* getFrame(const uint8_t identifier[4]) const;

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
    bool setFrame(const uint8_t identifier[4], uint32_t size, const uint8_t* data);

    /**
     * @brief Adds a new frame with the given identifier and data, regardless of existing frames.
     *
     * This always appends a new frame to the tag, even if other frames with the same identifier exist.
     *
     * @param identifier A 4 byte array representing the identifier of the frame.
     * @param size The size of the data to be set in the frame, in bytes.
     * @param data Pointer to the data to be copied into the frame.
     */
    void addFrame(const uint8_t identifier[4], uint32_t size, const uint8_t* data);

    /**
     * @brief Removes a specified frame from the tag.
     *
     * @param frame Pointer to the ID3v24Frame object to be removed from the tag.
     */
    void removeFrame(ID3v24Frame* frame);

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
     * @brief Retrieves the year from the tag (frame "TDRC" in ID3v2.4).
     *
     * @return Copy of the year string (first 4 characters) if found, otherwise an empty string.
     */
    std::string getYear() const;

    /**
     * @brief Sets the year in the tag (frame "TDRC").
     *
     * @param year The new year to be set (should be a 4-digit string).
     */
    void setYear(const std::string& year);

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
     * @return The track number as a string if found, otherwise an empty string.
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
     * @return The genre string (e.g., "(12)") if found, otherwise an empty string.
     */
    std::string getGenre() const;

    /**
     * @brief Sets the genre in the tag using ID3v1 genre number.
     *
     * @param genre The new genre to be set as a number (0-255).
     */
    void setGenre(uint8_t genre);

public:
    /**
     * @var ID3v24Header ID3v24::header
     *
     * @brief Class that holds ID3 version 2.4 fields of header data.
     */
    ID3v24Header header;

    /**
     * @var ID3v24ExtendedHeader* ID3v24::extendedHeader
     *
     * @brief Pointer to the extended header, if present. This can be null if the tag does not have an extended header.
     *
     * @note The extended header is dynamically allocated and managed.
     */
    ID3v24ExtendedHeader* extendedHeader;

    /**
     * @var ID3v24Footer* ID3v24::footer
     *
     * @brief Pointer to the optional footer, if present.
     *
     * @note The footer is dynamically allocated and managed.
     */
    ID3v24Footer* footer;

    /**
     * @var std::vector<ID3v24Frame*> ID3v24::frames
     *
     * @brief Vector containing pointers to ID3v2.4 frames.
     *
     * @note Frames are dynamically allocated and managed.
     */
    std::vector<ID3v24Frame*> frames;
};

#endif /* ID3V24_HPP */