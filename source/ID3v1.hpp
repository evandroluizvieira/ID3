#ifndef ID3V1_HPP
#define ID3V1_HPP

/**
 * @file ID3v1.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 28/06/2024
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the union ID3v1.
 */

#include "ID3v10.hpp"
#include "ID3v11.hpp"

/**
 * @union ID3v1
 *
 * @brief: ID3 tag version 1.
 *
 * @note This control an instance of ID3v10 or ID3v11, only one by time.
 *
 * @see ID3v10 specification in https://id3.org/ID3v1
 * @see ID3v11 specification in https://id3.org/ID3v1
 */
union ID3v1{
	public:
		/**
		 * @brief Default constructor that ensures all members are cleared.
		 */
		ID3v1();

		/**
		 * @brief: Destructor that calls ID3v1::clear().
		 */
		~ID3v1();

		/**
		 * @brief: Check if is the ID3 version 1.0.
		 *
		 * @return: true if is the version 1.0, false otherwise.
		 */
		bool isV10() const;

		/**
		 * @brief: Check if is the ID3 version 1.1.
		 *
		 * @return: true if is the version 1.1, false otherwise.
		 */
		bool isV11() const;

		/**
		 * @brief: Deallocate ID3v1::v10 and ID3v1::v11 if they are not null and clear their pointers.
		 */
		void clear();

	public:
		/**
		 * @var ID3v10* ID3v1::v10
		 *
		 * @brief Pointer to a ID3v10*, used to stores the ID3 version 1.0 data.
		 */
		ID3v10* v10;

		/**
		 * @var ID3v11* ID3v1::v11
		 *
		 * @brief Pointer to a ID3v11*, used to stores the ID3 version 1.1 data.
		 */
		ID3v11* v11;
};

#endif /* ID3V1_HPP */
