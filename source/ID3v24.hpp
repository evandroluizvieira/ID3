#ifndef ID3V24_HPP
#define ID3V24_HPP

#include "ID3v23.hpp"

/**
 * @class ID3v24Header
 *
 * @brief ID3 tag version 2.4 header derived from ID3v23Header.
 */
class ID3v24Header : public ID3v23Header{
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
 * @class ID3v24
 *
 * @brief ID3 tag version 2.4 class that holds information in ID3v24Header class.
 *
 * @note Not Completed.
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

	private:
		/**
		 * @var ID3v24Header ID3v24::header
		 *
		 * @brief Class that holds ID3 version 2.4 fields of header data.
		 */
		ID3v24Header header;
};

#endif /* ID3V24_HPP */
