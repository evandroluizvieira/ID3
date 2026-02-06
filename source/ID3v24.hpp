#ifndef ID3V24_HPP
#define ID3V24_HPP

/**
 * @file ID3v24.hpp
 *
 * @author Evandro L. Vieira
 *
 * @brief This file contains the definition of the class ID3v24, inheriting from ID3v23, for ID3v2.4 tags.
 */

#include "ID3v23.hpp"

#include <cstdint>

/**
 * @class ID3v24Header
 *
 * @brief ID3 tag version 2.4 header derived from ID3v23Header.
 */
class ID3v24Header : public ID3v23Header {
public:
	/**
	 * @brief Default constructor initializing the ID3v23Header and set major version to 4.
	 */
	ID3v24Header();

	/**
	 * @brief Default virtual destructor that calls ID3v23Header::~ID3v23Header().
	 */
	virtual ~ID3v24Header();

	/**
	 * @brief Set the footer present flag.
	 *
	 * @param hasFooter Whether to set or clear the footer present flag.
	 */
	void setFooterPresent(bool hasFooter);

	/**
	 * @brief Check if footer present flag is set.
	 *
	 * @return true if the footer present flag is set, false otherwise.
	 */
	bool hasFooter() const;
};

/**
 * @class ID3v24Footer
 *
 * @brief Represents the 10-byte footer of an ID3v2.4 tag.
 *
 * The footer is optional in ID3v2.4 and is used to help locate the tag from the end of the file.
 * It is a copy of the header, but with the identifier "3DI" instead of "ID3".
 *
 * @see https://id3.org/id3v2.4.0-structure
 */
class ID3v24Footer {
public:
	/**
	 * @brief Default constructor. Initializes the footer with default values.
	 */
	ID3v24Footer();

	/**
	 * @brief Destructor. Calls clear().
	 */
	~ID3v24Footer();

	/**
	 * @brief Sets the footer fields.
	 *
	 * @param flags Footer flags (should match header flags).
	 * @param size 4-byte synchsafe tag size.
	 */
	void setFooter(uint8_t flags, uint8_t size[4]);

	/**
	 * @brief Clears the footer to default values.
	 */
	void clear();

	/**
	 * @brief Serializes the footer to a 10-byte buffer.
	 *
	 * @param out Buffer to write the footer to (must be at least 10 bytes).
	 */
	void toBytes(uint8_t out[10]) const;

	/**
	 * @brief Loads the footer from a 10-byte buffer.
	 *
	 * @param in Buffer to read the footer from (must be at least 10 bytes).
	 */
	void fromBytes(const uint8_t in[10]);

	/**
	 * @brief Checks if a buffer contains a valid ID3v2.4 footer.
	 *
	 * @param in Buffer to check (must be at least 10 bytes).
	 * @return true if the buffer is a valid footer, false otherwise.
	 */
	static bool isFooter(const uint8_t in[10]);

	/**
	 * @var id
	 *
	 * @brief Footer identifier, always "3DI".
	 */
	char id[3];

	/**
	 * @var version_major
	 *
	 * @brief Major version, always 0x04 for ID3v2.4.
	 */
	uint8_t version_major;

	/**
	 * @var version_minor
	 *
	 * @brief Minor version, always 0x00 for ID3v2.4.0.
	 */
	uint8_t version_minor;

	/**
	 * @var flags
	 *
	 * @brief Footer flags (should match header flags).
	 */
	uint8_t flags;

	/**
	 * @var size
	 *
	 * @brief 4-byte synchsafe tag size (excluding header and footer).
	 */
	uint8_t size[4];
};

/**
 * @class ID3v24
 *
 * @brief ID3 tag version 2.4 class, inherits from ID3v23. Only overrides what is different (e.g., year field).
 */
class ID3v24 : public ID3v23 {
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
	 * @brief Retrieves the year from the tag using TDRC frame (ID3v2.4).
	 * @return Copy of the year string if found, otherwise an empty string.
	 */
	std::string getYear() const override;

	/**
	 * @brief Sets the year in the tag using TDRC frame (ID3v2.4).
	 *
	 * @param year The new year to be set.
	 */
	void setYear(const std::string& year) override;

	/**
	 * @brief Serializes the footer to a buffer (10 bytes).
	 *
	 * @param out Buffer to write the footer to (must be at least 10 bytes).
	 */
	void writeFooter(uint8_t* out) const;

	/**
	 * @brief Loads the footer from a buffer (10 bytes).
	 *
	 * @param in Buffer to read the footer from (must be at least 10 bytes).
	 */
	void readFooter(const uint8_t* in);

	/**
	 * @var header
	 *
	 * @brief Header for the ID3v2.4 tag.
	 */
	ID3v24Header header;

	/**
	 * @var footer
	 *
	 * @brief Footer for the ID3v2.4 tag (optional, only present if header flag is set).
	 */
	ID3v24Footer footer;
};

#endif /* ID3V24_HPP */
