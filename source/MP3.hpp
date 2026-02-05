#ifndef MP3_HPP
#define MP3_HPP

/**
 * @file MP3.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 29/01/2026
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the class MP3.
 */

#include "MP3Frame.hpp"

#include <cstdint>
#include <vector>

/**
 * @class MP3
 *
 * @brief MP3 class containing a sequence of MP3 frames.
 */
class MP3 {
	public:
		/**
		 * @brief Default constructor that initializes an empty frame list.
		 */
		MP3();

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~MP3();

		/**
		 * @brief Get the number of frames.
		 *
		 * @return Number of MP3 frames.
		 */
		uint32_t getFrameCount() const;

		/**
		 * @brief Get a frame at the given index.
		 *
		 * @param index Frame index (0-based).
		 *
		 * @return Reference to MP3Frame at index, or throws std::out_of_range if invalid.
		 */
		MP3Frame& getFrame(uint32_t index);

		/**
		 * @brief Get a frame at the given index (const).
		 *
		 * @param index Frame index (0-based).
		 *
		 * @return Const reference to MP3Frame at index, or throws std::out_of_range if invalid.
		 */
		const MP3Frame& getFrame(uint32_t index) const;

		/**
		 * @brief Add a frame to the sequence.
		 *
		 * @param frame Reference to MP3Frame to add.
		 */
		void addFrame(const MP3Frame& frame);

		/**
		 * @brief Remove a frame at the given index.
		 *
		 * @param index Frame index to remove (0-based).
		 *
		 * @return true if removed successfully, false if index invalid.
		 */
		bool removeFrame(uint32_t index);

		/**
		 * @brief Clear all frames.
		 */
		void clear();

		/**
		 * @brief Check if no frames exist.
		 *
		 * @return true if empty, false otherwise.
		 */
		bool isEmpty() const;

		/**
		 * @brief Get total audio duration in milliseconds.
		 *
		 * @return Duration in milliseconds.
		 */
		double getTotalDurationMs() const;

		/**
		 * @brief Get total size of all frames (including headers).
		 *
		 * @return Total size in bytes.
		 */
		uint64_t getTotalAudioSize() const;

		/**
		 * @brief Get all frames.
		 *
		 * @return Const reference to the frames vector.
		 */
		const std::vector<MP3Frame>& getFrames() const;

		/**
		 * @brief Get all frames (mutable).
		 *
		 * @return Reference to the frames vector.
		 */
		std::vector<MP3Frame>& getFrames();

	private:
		/**
		 * @var std::vector<MP3Frame> MP3::frames
		 *
		 * @brief Vector holding all MP3 frames.
		 */
		std::vector<MP3Frame> frames;
};

#endif /* MP3_HPP */
