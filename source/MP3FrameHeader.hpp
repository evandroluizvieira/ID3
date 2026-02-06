#ifndef MP3FRAMEHEADER_HPP
#define MP3FRAMEHEADER_HPP

/**
 * @file MP3FrameHeader.hpp
 *
 * @author Evandro L. Vieira
 *
 * @brief This file contains the definition of the class MP3FrameHeader and struct MP3FrameHeaderData.
 */

#include <cstdint>

/**
 * @struct MP3FrameHeaderData
 *
 * @brief MP3 frame header data structure (4 bytes).
 */
struct MP3FrameHeaderData {
	/**
	 * @var uint8_t MP3FrameHeaderData::data[4]
	 *
	 * @brief Array holding 4-byte raw frame header data.
	 */
	uint8_t data[4];
};

/**
 * @class MP3FrameHeader
 *
 * @brief MP3 frame header class that holds information in MP3FrameHeaderData structure.
 *
 * @note Frame header format (4 bytes):
 *       - Byte 0-1: Sync word (0xFFF) + MPEG version + Layer
 *       - Byte 2: Bitrate + Sample rate
 *       - Byte 3: Padding + Private bit + Side info + CRC
 */
class MP3FrameHeader {
	friend class MP3Frame;

	public:
		/**
		 * @enum MP3FrameHeader::MPEGVersion
		 *
		 * @brief MPEG version enumeration.
		 */
		enum MPEGVersion {
			MPEG25 = 0,  /**< MPEG version 2.5 */
			Reserved = 1, /**< Reserved */
			MPEG2 = 2,   /**< MPEG version 2.0 */
			MPEG1 = 3    /**< MPEG version 1.0 */
		};

		/**
		 * @enum MP3FrameHeader::Layer
		 *
		 * @brief Layer enumeration.
		 */
		enum Layer {
			LayerReserved = 0, /**< Reserved layer */
			LayerIII = 1,      /**< Layer III */
			LayerII = 2,       /**< Layer II */
			LayerI = 3         /**< Layer I */
		};

		/**
		 * @enum MP3FrameHeader::BitrateIndex
		 *
		 * @brief Bitrate index (0-15) for different MPEG versions and layers.
		 */
		enum BitrateIndex {
			BR_Free = 0,
			BR_32 = 1,
			BR_40 = 2,
			BR_48 = 3,
			BR_56 = 4,
			BR_64 = 5,
			BR_80 = 6,
			BR_96 = 7,
			BR_112 = 8,
			BR_128 = 9,
			BR_160 = 10,
			BR_192 = 11,
			BR_224 = 12,
			BR_256 = 13,
			BR_320 = 14,
			BR_Bad = 15
		};

		/**
		 * @enum MP3FrameHeader::SampleRate
		 *
		 * @brief Sample rate enumeration.
		 */
		enum SampleRate {
			SR_44100 = 0,	/**< 44.1 kHz */
			SR_48000 = 1,	/**< 48 kHz */
			SR_32000 = 2,	/**< 32 kHz */
			SR_Reserved = 3	/**< Reserved */
		};

		/**
		 * @brief Default constructor that initializes a valid frame header.
		 */
		MP3FrameHeader();

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~MP3FrameHeader();

		/**
		 * @brief Check if sync word is valid (0xFFF).
		 *
		 * @return true if valid, false otherwise.
		 */
		bool isValidSync() const;

		/**
		 * @brief Get MPEG version.
		 *
		 * @return MPEG version (1, 2, or 2.5).
		 */
		MPEGVersion getMPEGVersion() const;

		/**
		 * @brief Set MPEG version.
		 *
		 * @param version MPEG version to set.
		 */
		void setMPEGVersion(MPEGVersion version);

		/**
		 * @brief Get Layer.
		 *
		 * @return Layer (I, II, or III).
		 */
		Layer getLayer() const;

		/**
		 * @brief Set Layer.
		 *
		 * @param layer Layer to set.
		 */
		void setLayer(Layer layer);

		/**
		 * @brief Check if CRC protection is used.
		 *
		 * @return true if CRC is used, false otherwise.
		 */
		bool hasCRC() const;

		/**
		 * @brief Set CRC protection flag.
		 *
		 * @param use true to enable CRC, false to disable.
		 */
		void setCRC(bool use);

		/**
		 * @brief Get bitrate index.
		 *
		 * @return Bitrate index (0-15).
		 */
		BitrateIndex getBitrateIndex() const;

		/**
		 * @brief Set bitrate index.
		 *
		 * @param index Bitrate index (0-15).
		 */
		void setBitrateIndex(BitrateIndex index);

		/**
		 * @brief Get bitrate in kbps based on MPEG version and layer.
		 *
		 * @return Bitrate in kbps, or 0 if invalid.
		 */
		uint16_t getBitrate() const;

		/**
		 * @brief Get sample rate index.
		 *
		 * @return Sample rate index (0-3).
		 */
		SampleRate getSampleRateIndex() const;

		/**
		 * @brief Set sample rate index.
		 *
		 * @param rate Sample rate index to set.
		 */
		void setSampleRateIndex(SampleRate rate);

		/**
		 * @brief Get sample rate in Hz.
		 *
		 * @return Sample rate in Hz (44100, 48000, or 32000).
		 */
		uint32_t getSampleRate() const;

		/**
		 * @brief Check if frame has padding.
		 *
		 * @return true if padded, false otherwise.
		 */
		bool hasPadding() const;

		/**
		 * @brief Set padding flag.
		 *
		 * @param padded true to add padding, false otherwise.
		 */
		void setPadding(bool padded);

		/**
		 * @brief Get frame size in bytes (header + data).
		 *
		 * @return Frame size in bytes, or 0 if invalid.
		 */
		uint16_t getFrameSize() const;

		/**
		 * @brief Get the raw 4-byte header data.
		 *
		 * @return Pointer to the raw header data.
		 */
		const uint8_t* getRawData() const;

	private:
		/**
		 * @var MP3FrameHeaderData MP3FrameHeader::data
		 *
		 * @brief Raw 4-byte header data.
		 */
		MP3FrameHeaderData data;
};

#endif /* MP3FRAMEHEADER_HPP */
