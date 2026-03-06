#ifndef EMP3FILE_HPP
#define EMP3FILE_HPP

/**
 * @file MP3File.hpp
 *
 * @author Evandro L. Vieira
 *
 * @brief This file contains the definition of the class MP3File.
 */

#include "File.hpp"
#include "ID3.hpp"

/**
 * @brief MP3 file class that holds loaded ID3 tags and/or mp3 data.
 */
class MP3File : public File{
	public:
		/**
		 * @brief Default constructor.
		 */
		MP3File();

		/**
		 * @brief Default virtual destructor that calls File::~File().
		 */
		virtual ~MP3File();

		/**
		 * @brief Check if loaded data contains ID3 tag version 1.0.
		 *
		 * @return true if contains, false otherwise.
		 */
		bool hasID3v10() const;

		/**
		 * @brief Check if loaded data contains ID3 tag version 1.1.
		 *
		 * @return true if contains, false otherwise.
		 */
		bool hasID3v11() const;

		/**
		 * @brief Check if loaded data contains ID3 tag version 2.0.
		 *
		 * @return true if contains, false otherwise.
		 */
		bool hasID3v20() const;

		/**
		 * @brief Check if loaded data contains ID3 tag version 2.3.
		 *
		 * @return true if contains, false otherwise.
		 */
		bool hasID3v23() const;

		/**
		 * @brief Check if loaded data contains ID3 tag version 2.4.
		 *
		 * @return true if contains, false otherwise.
		 */
		bool hasID3v24() const;

		/**
		 * @brief Get ID3 Tags if they exist.
		 *
		 * @return Copy of ID3 tag version 1 and/or version 2.
		 */
		ID3 getID3() const;

		/**
		 * @brief Set the given ID3 tags to the file.
		 *
		 * @param id3 The ID3 tags to set.
		 */
		void setID3(const ID3& id3);

		/**
		 * @brief Remove all ID3 tags.
		 */
		void removeTags();
};

#endif /* EMP3FILE_HPP */
