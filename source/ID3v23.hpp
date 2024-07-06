#ifndef ID3V23_HPP
#define ID3V23_HPP

#include "ID3v2.hpp"

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
 * @class ID3v23
 *
 * @brief ID3 tag version 2.3 class that holds information in ID3v23Header class.
 *
 * @note Not Completed.
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

	private:
		/**
		 * @var ID3v23Header ID3v23::header
		 *
		 * @brief Class that holds ID3 version 2.3 fields of header data.
		 */
		ID3v23Header header;
};

#endif /* ID3V23_HPP */
