#ifndef FILE_HPP
#define FILE_HPP

/**
 * @file File.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 06/07/2024
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the class File.
 */

#include <cstdint>

/**
 * @brief File class that holds binary data.
 */
class File{
	friend class MP3File;

	public:
		/**
		 * @brief Default constructor that initializes the object with empty data.
		 */
		File();

		/**
		 * @brief Default virtual destructor that calls File::clear().
		 */
		virtual ~File();

		/**
		 * @brief Operator to get or set the byte at the given index.
		 *
		 * @param index The index of the byte to access in File::data.
		 *
		 * @return A reference to the byte at the given index in File::data.
		 *
		 * @throws std::out_of_range If the index is out of bounds (index >= size).
		 */
		uint8_t& operator[](uint64_t index);

		/**
		 * @brief Function that releases all data, clears the content of File::data and sets size to 0.
		 */
		void clear();

		/**
		 * @brief Check if data is empty.
		 *
		 * @return true if data is empty, false otherwise.
		 */
		bool isEmpty() const;

		/**
		 * @brief Load data from file and store into File::data if File::data is empty.
		 *
		 * @param file The file path to load data from.
		 *
		 * @return true if successful, false otherwise.
		 */
		bool load(const char* file);

		/**
		 * @brief Write data into the given file.
		 *
		 * @param file The file path to save data to.
		 *
		 * @return true if successful, false otherwise.
		 */
		bool save(const char* file);

	private:
		/**
		 * @var uint8_t* File::data
		 *
		 * @brief Pointer to the array holding the binary data.
		 */
		uint8_t* data;

		/**
		 * @var uint64_t File::size
		 *
		 * @brief The size of the data array.
		 */
		uint64_t size;
};

#endif /* FILE_HPP */
