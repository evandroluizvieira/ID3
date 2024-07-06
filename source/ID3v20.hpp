#ifndef ID3V20_HPP
#define ID3V20_HPP

#include "ID3v2.hpp"

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

		/**
		 * @brief Get the tag size as a 32-bit integer (synchsafe encoding).
		 *
		 * @return The size of the tag.
		 */
		uint32_t getSize() const;

		/**
		 * @brief Set the tag size from a 32-bit integer (synchsafe encoding).
		 *
		 * @param size The new size to set for the tag.
		 */
		void setSize(uint32_t size);
};

/**
 * @class ID3v20
 *
 * @brief ID3 tag version 2.0 class that holds information in ID3v20Header class.
 *
 * @note Not Completed.
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

	private:
		/**
		 * @var ID3v20Header ID3v20::header
		 *
		 * @brief Class that holds ID3 version 2.0 fields of header data.
		 */
		ID3v20Header header;
};

#endif /* ID3V20_HPP */
