#ifndef EMP3FILE_HPP
#define EMP3FILE_HPP

#include "File.hpp"
#include "ID3.hpp"

/*
 * @brief: MP3 file class that holds loaded ID3 tags and/or mp3 'data'.
 */
class MP3File : public File{
	public:
		/*
		 * @brief: Default constructor.
		 */
		MP3File();

		/*
		 * @brief: Default virtual destructor that call 'EFile::~EFile()'.
		 */
		virtual ~MP3File();

	public:
		/*
		 * @brief: Check if loaded 'data' contains ID3 tag version 1.0.
		 * @return: 'true' in success and 'false' otherwise.
		 */
		bool hasID3v10() const;

		/*
		 * @brief: Check if loaded 'data' contains ID3 tag version 1.1.
		 * @return: 'true' in success and 'false' otherwise.
		 */
		bool hasID3v11() const;

		/*
		 * @brief: Check if loaded 'data' contains ID3 tag version 2.2.
		 * @return: 'true' in success and 'false' otherwise.
		 */
		bool hasID3v22() const;

		/*
		 * @brief: Check if loaded 'data' contains ID3 tag version 2.3.
		 * @return: 'true' in success and 'false' otherwise.
		 */
		bool hasID3v23() const;

		/*
		 * @brief: Check if loaded 'data' contains ID3 tag version 2.3.
		 * @return: 'true' in success and 'false' otherwise.
		 */
		bool hasID3v24() const;

		/*
		 * @brief: Get ID3 Tags if exists.
		 * @return: Copy of ID3 tag version 1 and or version 2.
		 */
		ID3 getID3() const;

		/*
		 * @brief: Check if loaded 'data' contains ID3 tag version 1.0.
		 * @return: 'true' in success and 'false' otherwise.
		 */
		void setID3(const ID3& id3);

		/*
		 * @brief: Remove all ID3 tags.
		 */
		void removeTags();
};

#endif /* EMP3FILE_HPP */
