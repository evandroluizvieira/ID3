#ifndef FILE_HPP
#define FILE_HPP

#include <cstdint>

/*
 * @description: File class that holds binary 'data'.
 */
class File{
		friend class MP3File;

	public:
		/*
		 * @description: Default constructor.
		 * @return: None.
		 */
		File();

		/*
		 * @brief: Default virtual destructor that call 'clear()'.
		 * @return: None.
		 */
		virtual ~File();

		/*
		 * @description: Operator to get the byte of the given 'index'.
		 * @return: Copy of the data in the 'index' position if 'index' is valid.
		 */
		uint8_t operator[](unsigned int index);


		/*
		 * @brief: Function that releases all 'data'.
		 * @return: None.
		 */
		void clear();

		/*
		 * @brief: Check if 'data' is empty.
		 * @return: true if it is empty and false otherwise.
		 */
		bool isEmpty() const;

		/*
		 * @brief: Load data from 'file' and store into 'data' if 'isEmpty()' is true.
		 * @return: true if success and false otherwise.
		 */
		bool load(const char* file);

		/*
		 * @brief: Write 'data' into 'file'.
		 * @return: true if success and false otherwise.
		 */
		bool save(const char* file);

	private:
		uint8_t* data;
		uint64_t size;
};

#endif /* FILE_HPP */
