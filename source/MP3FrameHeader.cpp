#include "MP3FrameHeader.hpp"

#include <cstring>

namespace{

/**
 * @brief Bitrate lookup table in kbps indexed by [MPEGVersion][layerIndex][bitrateIndex].
 *
 * First dimension uses MP3FrameHeader::MPEGVersion values (MPEG25=0, Reserved=1, MPEG2=2, MPEG1=3).
 * Second dimension uses layer index (0=LayerI, 1=LayerII, 2=LayerIII).
 * Third dimension uses the 4-bit bitrate index from the frame header (0-15).
 *
 * A value of 0 indicates free format or a bad/reserved index.
 *
 * @note Based on the MPEG audio specification bitrate table (V1=MPEG1, V2=MPEG2 and MPEG2.5).
 */
static const uint16_t BIT_RATE_TABLE[4][3][16] = {
	{ // MPEG 2.5 — same bitrates as MPEG 2
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0}, // L1  = V2,L1
		{0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160, 0}, // L2  = V2,L2&L3
		{0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160, 0}  // L3  = V2,L2&L3
	},
	{ // Reserved — all zeroes
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{ // MPEG 2
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0}, // L1  = V2,L1
		{0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160, 0}, // L2  = V2,L2&L3
		{0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160, 0}  // L3  = V2,L2&L3
	},
	{ // MPEG 1
		{0,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0}, // L1 = V1,L1
		{0,  32,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, 0}, // L2 = V1,L2
		{0,  32,  40,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 0}  // L3 = V1,L3
	}
};

/**
 * @brief Sample rate lookup table in Hz indexed by [MPEGVersion][sampleRateIndex].
 *
 * First dimension uses MP3FrameHeader::MPEGVersion values (MPEG25=0, Reserved=1, MPEG2=2, MPEG1=3).
 * Second dimension uses the 2-bit sample rate index from the frame header (0-3).
 *
 * A value of 0 indicates a reserved or invalid index.
 *
 * @note Based on the MPEG audio specification sample rate table.
 */
static const uint32_t SAMPLE_RATE_TABLE[4][4] = {
	{11025, 12000,  8000, 0}, // MPEG 2.5
	{    0,     0,     0, 0}, // Reserved
	{22050, 24000, 16000, 0}, // MPEG 2
	{44100, 48000, 32000, 0}  // MPEG 1
};

}

MP3FrameHeader::MP3FrameHeader() {
	std::memset(&data, 0, sizeof(MP3FrameHeaderData));
	data.data[0] = 0xFF;
	data.data[1] = 0xFB;
	data.data[2] = (9 << 4) | 0;
	data.data[3] = 0;
}

MP3FrameHeader::~MP3FrameHeader() {
	std::memset(&data, 0, sizeof(MP3FrameHeaderData));
}

bool MP3FrameHeader::isValidSync() const {
	return (data.data[0] == 0xFF) && ((data.data[1] & 0xE0) == 0xE0);
}

MP3FrameHeader::MPEGVersion MP3FrameHeader::getMPEGVersion() const {
	uint8_t version = (data.data[1] >> 3) & 0x03;
	return static_cast<MPEGVersion>(version);
}

void MP3FrameHeader::setMPEGVersion(MPEGVersion version) {
	data.data[1] = (data.data[1] & 0xE7) | ((version & 0x03) << 3);
}

MP3FrameHeader::Layer MP3FrameHeader::getLayer() const {
	uint8_t layer = (data.data[1] >> 1) & 0x03;
	return static_cast<Layer>(layer);
}

void MP3FrameHeader::setLayer(Layer layer) {
	data.data[1] = (data.data[1] & 0xF9) | ((layer & 0x03) << 1);
}

bool MP3FrameHeader::hasCRC() const {
	return (data.data[1] & 0x01) == 0;
}

void MP3FrameHeader::setCRC(bool use) {
	if (use) {
		data.data[1] &= ~0x01;
	} else {
		data.data[1] |= 0x01;
	}
}

MP3FrameHeader::BitrateIndex MP3FrameHeader::getBitrateIndex() const {
	uint8_t index = (data.data[2] >> 4) & 0x0F;
	return static_cast<BitrateIndex>(index);
}

void MP3FrameHeader::setBitrateIndex(BitrateIndex index) {
	data.data[2] = (data.data[2] & 0x0F) | ((index & 0x0F) << 4);
}

uint16_t MP3FrameHeader::getBitrate() const {
	MPEGVersion version = getMPEGVersion();
	Layer layer = getLayer();
	BitrateIndex index = getBitrateIndex();

	if (version == Reserved || layer == LayerReserved) {
		return 0;
	}

	// Layer is stored as: 0=Reserved, 1=LayerIII, 2=LayerII, 3=LayerI
	// Table is indexed as: 0=LayerI, 1=LayerII, 2=LayerIII
	uint8_t layerIndex = (layer == LayerI) ? 0 : (layer == LayerII) ? 1 : 2;

	return BIT_RATE_TABLE[version][layerIndex][index];
}

MP3FrameHeader::SampleRate MP3FrameHeader::getSampleRateIndex() const {
	uint8_t rate = (data.data[2] >> 2) & 0x03;
	return static_cast<SampleRate>(rate);
}

void MP3FrameHeader::setSampleRateIndex(SampleRate rate) {
	data.data[2] = (data.data[2] & 0xF3) | ((rate & 0x03) << 2);
}

uint32_t MP3FrameHeader::getSampleRate() const {
	MPEGVersion version = getMPEGVersion();
	SampleRate sampleRateIndex = getSampleRateIndex();

	if (version == Reserved || sampleRateIndex == SR_Reserved) {
		return 0;
	}

	return SAMPLE_RATE_TABLE[version][sampleRateIndex];
}

bool MP3FrameHeader::hasPadding() const {
	return (data.data[2] & 0x02) != 0;
}

void MP3FrameHeader::setPadding(bool padded) {
	if (padded) {
		data.data[2] |= 0x02;
	} else {
		data.data[2] &= ~0x02;
	}
}

uint16_t MP3FrameHeader::getFrameSize() const {
	if (!isValidSync()) {
		return 0;
	}

	uint32_t sampleRate = getSampleRate();
	uint16_t bitrate = getBitrate();

	if (sampleRate == 0 || bitrate == 0) {
		return 0;
	}

	Layer layer = getLayer();
	uint16_t frameSize = 0;

	if (layer == LayerIII || layer == LayerII) {
		// Frame size = (144 * bitrate / sampleRate) + padding
		frameSize = (144000 * bitrate / sampleRate) + (hasPadding() ? 1 : 0);
	} else if (layer == LayerI) {
		// Frame size = (12 * bitrate / sampleRate + padding) * 4
		frameSize = ((12000 * bitrate / sampleRate) + (hasPadding() ? 1 : 0)) * 4;
	}

	return frameSize;
}

const uint8_t* MP3FrameHeader::getRawData() const {
	return data.data;
}
