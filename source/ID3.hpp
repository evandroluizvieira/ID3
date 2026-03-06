#ifndef ID3_HPP
#define ID3_HPP

/**
 * @file ID3.hpp
 *
 * @author Evandro L. Vieira
 *
 * @brief This file contains the definition of the class ID3.
 */

#include "ID3v1.hpp"
#include "ID3v2.hpp"

/**
 * @class ID3
 *
 * @brief ID3 class representing ID3 tags version 1 and 2.
 *
 * @see ID3v1
 * @see ID3v2
 *
 * @note ID3 class manages instances of ID3v1 and ID3v2 simultaneously.
 */
class ID3{
	public:
		/**
		 * @brief Default constructor that initializes the object with empty data.
		 */
		ID3();

		/**
		 * @brief Default virtual destructor.
		 */
		virtual ~ID3();

	public:
		/**
		 * @var ID3v1* ID3::v1
		 *
		 * @brief Pointer to ID3v1 instance for ID3 tag version 1.
		 */
		ID3v1 v1;

		/**
		 * @var ID3v2* ID3::v2
		 *
		 * @brief Pointer to ID3v2 instance for ID3 tag version 2.
		 */
		ID3v2 v2;
};

#endif /* ID3_HPP */
