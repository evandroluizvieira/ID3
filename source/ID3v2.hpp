#ifndef ID3V2_HPP
#define ID3V2_HPP

/**
 * @file ID3v2.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 28/06/2024
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the union ID3v2 and class ID3v2HeaderBase.
 */

#include <cstdint>

class ID3v20;
class ID3v23;
class ID3v24;

/**
 * @class ID3v2 Header base header for versions 2.0, 2.3 and 2.4.
 *
 * @note This class could be a struct as it always occupies 10 bytes in the file.
 *
 * @see ID3v20Header
 * @see ID3v23Header
 * @see ID3v24Header
 */
class ID3v2HeaderBase{
	public:
		/**
		 * @brief Default constructor that starts the ID3v2HeaderBase::tag with "ID3" and ensures other members are cleared.
		 */
		ID3v2HeaderBase();

		/**
 	 	 *@brief Virtual destructor that clears all members.
		 */
		virtual ~ID3v2HeaderBase();

		/**
		 * @brief: Check if unsynchronization flag is set.
		 *
		 * @return: true if ID3v2Header::flag bit 7 is set and false otherwise.
		 */
		bool isUnsynchronized() const;

		/**
		 * @brief Set the unsynchronization flag.
		 *
		 * @param useUnsynchronization When true, sets bit 7 of ID3v2Header::flag; when false, clears it.
		 *
		 * @note Checks the ID3 official documentation of each version for details .
		 */
		void setUnsynchronized(bool useUnsynchronization);

	public:
		/**
		 * @var uint8_t ID3v2HeaderBase::tag[3]
		 *
		 * @brief Tag identifier, always set "ID3" for ID3v2.0, ID3v2.3, and ID3v2.4.
		 */
		uint8_t tag[3];

		/**
		 * @var uint8_t ID3v2HeaderBase::major_version
		 *
		 * @brief Major version number of the ID3v2 tag.
		 */
		uint8_t major_version;

		/**
		 * @var uint8_t ID3v2HeaderBase::revision_number
		 *
		 * @brief Revision number of the ID3v2 tag.
		 */
		uint8_t revision_number;

		/**
		 * @var uint8_t ID3v2HeaderBase::flags
		 *
		 * @brief Flags byte indicating various attributes of the ID3v2 tag.
		 */
		uint8_t flags;

		/**
		 * @var uint8_t ID3v2HeaderBase::size[4]
		 *
		 * @brief ID3 tag size, 28-bit encoded as 4 bytes.
		 */
		uint8_t size[4];
};

/**
 * @union ID3v2
 *
 * @brief: ID3 tag version 2.
 *
 * @note This control an instance of ID3v20, ID3v23 or ID3v24, only one by time.
 *
 * @see ID3v20 specification in https://id3.org/id3v2-00
 * @see ID3v23 specification in https://id3.org/d3v2.3.0
 * @see ID3v24 specification in https://id3.org/id3v2.4.0-structure
 */
union ID3v2{
	public:
		/**
		 * @brief Default constructor that ensures all members are cleared.
		 */
		ID3v2();

		/**
		 * @brief: Destructor that calls ID3v2::clear().
		 */
		~ID3v2();

		/**
		 * @brief: Check if is the ID3 version 2.0.
		 *
		 * @return: true if is the version 2.0, false otherwise.
		 */
		bool isV20() const;

		/**
		 * @brief: Check if is the ID3 version 2.3.
		 *
		 * @return: true if is the version 2.3, false otherwise.
		 */
		bool isV23() const;

		/**
		 * @brief: Check if is the ID3 version 2.4.
		 *
		 * @return: true if is the version 2.4, false otherwise.
		 */
		bool isV24() const;

		/**
		 * @brief: Deallocate ID3v20::v20, ID3v23::v23 and ID3v24::v24 if they are not null and clear their pointers.
		 */
		void clear();

	public:
		/**
		 * @var ID3v20* ID3v20::v20
		 *
		 * @brief Pointer to a ID3v20*, used to stores the ID3 version 2.0 data.
		 */
		ID3v20* v20;

		/**
		 * @var ID3v23* ID3v23::v23
		 *
		 * @brief Pointer to a ID3v23*, used to stores the ID3 version 2.3 data.
		 */
		ID3v23* v23;

		/**
		 * @var ID3v24* ID3v24::v24
		 *
		 * @brief Pointer to a ID3v24*, used to stores the ID3 version 2.4 data.
		 */
		ID3v24* v24;
};

#endif /* ID3V2_HPP */
