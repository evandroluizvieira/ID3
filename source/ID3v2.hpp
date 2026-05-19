#ifndef ID3V2_HPP
#define ID3V2_HPP

/**
 * @file ID3v2.hpp
 *
 * @author Evandro L. Vieira
 *
 * @brief This file contains the definition of the union ID3v2 and class ID3v2HeaderBase.
 */

#include <cstdint>

class ID3v20;
class ID3v23;
class ID3v24;

/**
 * @class ID3v2Header
 * 
 * @brief Header for versions 2.0, 2.3 and 2.4.
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
		 * @brief Set the experimental indicator flag.
		 *
		 * @param isExperimental When true, sets bit 5 of the flags byte; when false, clears it.
		 */
		void setExperimental(bool isExperimental);

		/**
		 * @brief Check if experimental indicator flag is set.
		 *
		 * @return true if bit 5 of the flags byte is set, false otherwise.
		 */
		bool isExperimental() const;

				/**
		 * @brief Set the extended header flag.
		 *
		 * @param hasExtendedHeader When true, sets bit 6 of the flags byte; when false, clears it.
		 */
		void setExtendedHeader(bool hasExtendedHeader);

		/**
		 * @brief Check if extended header flag is set.
		 *
		 * @return true if bit 6 of the flags byte is set, false otherwise.
		 */
		bool hasExtendedHeader() const;

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

		/**
		 * @brief Retrieves the tag size.
		 *
		 * @note The size of the ID3 tag is stored across four bytes where each byte's most significant bit (bit 7) is set to zero.
		 *       Encoded as a 32-bit integer using synchsafe encoding, this encoding scheme uses 28 effective bits to represent the size.
		 *
		 * @return The size of the tag.
		 */
		uint32_t getTagSize() const;

		/**
		 * @brief Set the tag size.
		 *
		 * @note The size of the ID3 tag is stored across four bytes where each byte's most significant bit (bit 7) is set to zero.
		 *       Encoded as a 32-bit integer using synchsafe encoding, this encoding scheme uses 28 effective bits to represent the size.
		 *
		 * @param size The new size to set for the tag.
		 */
		void setTagSize(uint32_t size);

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
 * @class ID3v2FrameHeaderBase
 *
 * @brief Frame headers base class for versions 2.3 and 2.4.
 *
 * @note This class could be a struct as it always occupies 10 bytes in the file.
 *
 * @see ID3v23FrameHeader
 * @see ID3v24FrameHeader
 */
class ID3v2FrameHeaderBase {
public:
    /**
     * @brief Default constructor that clears all members.
     */
    ID3v2FrameHeaderBase();

    /**
     * @brief Virtual destructor that clears all members.
     */
    virtual ~ID3v2FrameHeaderBase();

    /**
     * @brief Set the tag alter preservation flag.
     *
     * @param preserve When true, sets bit 6 of the first flag byte; when false, clears it.
     */
    void setTagAlterPreservation(bool preserve);

    /**
     * @brief Check if the tag alter preservation flag is set.
     *
     * @return true if bit 6 of the first flag byte is set, false otherwise.
     */
    bool getTagAlterPreservation() const;

    /**
     * @brief Set the file alter preservation flag.
     *
     * @param preserve When true, sets bit 5 of the first flag byte; when false, clears it.
     */
    void setFileAlterPreservation(bool preserve);

    /**
     * @brief Check if the file alter preservation flag is set.
     *
     * @return true if bit 5 of the first flag byte is set, false otherwise.
     */
    bool getFileAlterPreservation() const;

    /**
     * @brief Retrieves the size of the frame excluding the header.
     *
     * @note This method is pure virtual because the interpretation of the
     *       four size bytes differs between ID3v2.3 (normal integer) and
     *       ID3v2.4 (synchsafe integer). Derived classes must implement it.
     *
     * @return The size of the frame data (in bytes).
     */
    virtual uint32_t getFrameSize() const = 0;

    /**
     * @brief Sets the size of the frame excluding the header.
     *
     * @param size The new size of the frame (in bytes).
     *
     * @note Pure virtual because the storage format differs between versions.
     */
    virtual void setFrameSize(uint32_t size) = 0;

    /**
     * @brief Set the read‑only flag.
     *
     * @param readOnly When true, sets the read‑only flag; when false, clears it.
     *
     * @note Pure virtual because the flag position differs between versions.
     */
    virtual void setReadOnly(bool readOnly) = 0;

    /**
     * @brief Check if the read‑only flag is set.
     *
     * @return true if the read‑only flag is set, false otherwise.
     */
    virtual bool isReadOnly() const = 0;

    /**
     * @brief Set the compression flag.
     *
     * @param compressed When true, sets the compression flag; when false, clears it.
     */
    virtual void setCompressed(bool compressed) = 0;

    /**
     * @brief Check if the compression flag is set.
     *
     * @return true if the compression flag is set, false otherwise.
     */
    virtual bool isCompressed() const = 0;

    /**
     * @brief Set the encryption flag.
     *
     * @param encrypted When true, sets the encryption flag; when false, clears it.
     */
    virtual void setEncrypted(bool encrypted) = 0;

    /**
     * @brief Check if the encryption flag is set.
     *
     * @return true if the encryption flag is set, false otherwise.
     */
    virtual bool isEncrypted() const = 0;

    /**
     * @brief Set the grouping identity flag.
     *
     * @param group When true, sets the grouping identity flag; when false, clears it.
     */
    virtual void setGroupingIdentity(bool group) = 0;

    /**
     * @brief Check if the grouping identity flag is set.
     *
     * @return true if the grouping identity flag is set, false otherwise.
     */
    virtual bool isGroupingIdentity() const = 0;

public:
    /**
     * @var uint8_t ID3v2FrameHeaderBase::identifier[4]
     *
     * @brief 4 character identifier for the frame type (e.g., "TIT2", "COMM").
     */
    uint8_t identifier[4];

    /**
     * @var uint8_t ID3v2FrameHeaderBase::size[4]
     *
     * @brief 4 byte size descriptor of the frame (interpretation depends on version).
     */
    uint8_t size[4];

    /**
     * @var uint8_t ID3v2FrameHeaderBase::flags[2]
     *
     * @brief 2 byte flags for the frame (bits are version‑specific).
     */
    uint8_t flags[2];
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
