#ifndef MP3FRAME_HPP
#define MP3FRAME_HPP

/**
 * @file MP3Frame.hpp
 *
 * @author Evandro L. Vieira
 *
 * @date 29/01/2026
 *
 * @version 1.0
 *
 * @brief This file contains the definition of the class MP3Frame.
 */

#include "MP3FrameHeader.hpp"

#include <cstdint>
#include <vector>

/**
 * @class MP3Frame
 *
 * @brief MP3 frame class containing frame header and audio data.
 *
 * @note A frame consists of:
 *       - 4-byte header (MP3FrameHeader)
 *       - Audio data (variable size depending on bitrate and sample rate)
 */
class MP3Frame {
	public:
		/**
		 * @brief Default constructor that initializes an empty frame.
		 */
		MP3Frame();

		/**
		 * @brief Constructor with frame header and data.
		 *
		 * @param header Reference to MP3FrameHeader.
		 * @param frameData Pointer to frame data bytes.
		 * @param dataSize Size of frame data in bytes.
		 */
		MP3Frame(const MP3FrameHeader& header, const uint8_t* frameData, uint16_t dataSize);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~MP3Frame();

		/**
		 * @brief Get the frame header.
		 *
		 * @return Reference to the MP3FrameHeader.
		 */
		MP3FrameHeader& getHeader();

		/**
		 * @brief Get the frame header (const).
		 *
		 * @return Const reference to the MP3FrameHeader.
		 */
		const MP3FrameHeader& getHeader() const;

		/**
		 * @brief Set the frame header.
		 *
		 * @param header Reference to new MP3FrameHeader.
		 */
		void setHeader(const MP3FrameHeader& header);

		/**
		 * @brief Get frame data.
		 *
		 * @return Pointer to frame data bytes.
		 */
		uint8_t* getData();

		/**
		 * @brief Get frame data (const).
		 *
		 * @return Const pointer to frame data bytes.
		 */
		const uint8_t* getData() const;

		/**
		 * @brief Set frame data.
		 *
		 * @param data Pointer to data bytes.
		 * @param size Size of data in bytes.
		 */
		void setData(const uint8_t* data, uint16_t size);

		/**
		 * @brief Get the size of frame data (without header).
		 *
		 * @return Data size in bytes.
		 */
		uint16_t getDataSize() const;

		/**
		 * @brief Get total frame size (header + data).
		 *
		 * @return Total frame size in bytes (4 + data size).
		 */
		uint16_t getTotalFrameSize() const;

		/**
		 * @brief Check if frame is valid.
		 *
		 * @return true if header has valid sync and data size matches expected.
		 */
		bool isValid() const;

		/**
		 * @brief Get frame duration in milliseconds.
		 *
		 * @return Duration in milliseconds (typically ~26ms for 1152 samples at 44.1kHz).
		 */
		double getFrameDurationMs() const;

	private:
		/**
		 * @var MP3FrameHeader MP3Frame::header
		 *
		 * @brief Frame header.
		 */
		MP3FrameHeader header;

		/**
		 * @var std::vector<uint8_t> MP3Frame::audioData
		 *
		 * @brief Frame audio data.
		 */
		std::vector<uint8_t> audioData;
};

#endif /* MP3FRAME_HPP */
