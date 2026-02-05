#include "MP3Frame.hpp"

#include <cstring>

MP3Frame::MP3Frame() : header(), audioData() {
	// Empty constructor
}

MP3Frame::MP3Frame(const MP3FrameHeader& header, const uint8_t* frameData, uint16_t dataSize)
	: header(header), audioData() {
	if (frameData != nullptr && dataSize > 0) {
		audioData.resize(dataSize);
		std::memcpy(audioData.data(), frameData, dataSize);
	}
}

MP3Frame::~MP3Frame() {
	audioData.clear();
}

MP3FrameHeader& MP3Frame::getHeader() {
	return header;
}

const MP3FrameHeader& MP3Frame::getHeader() const {
	return header;
}

void MP3Frame::setHeader(const MP3FrameHeader& newHeader) {
	header = newHeader;
}

uint8_t* MP3Frame::getData() {
	return audioData.empty() ? nullptr : audioData.data();
}

const uint8_t* MP3Frame::getData() const {
	return audioData.empty() ? nullptr : audioData.data();
}

void MP3Frame::setData(const uint8_t* data, uint16_t size) {
	audioData.clear();
	if (data != nullptr && size > 0) {
		audioData.resize(size);
		std::memcpy(audioData.data(), data, size);
	}
}

uint16_t MP3Frame::getDataSize() const {
	return static_cast<uint16_t>(audioData.size());
}

uint16_t MP3Frame::getTotalFrameSize() const {
	return 4 + getDataSize(); // 4 bytes header + data
}

bool MP3Frame::isValid() const {
	if (!header.isValidSync()) {
		return false;
	}

	uint16_t expectedFrameSize = header.getFrameSize();
	uint16_t actualDataSize = getDataSize();

	// Check if actual data matches expected size (frame size - header)
	return (expectedFrameSize > 4) && (actualDataSize == (expectedFrameSize - 4));
}

double MP3Frame::getFrameDurationMs() const {
	uint32_t sampleRate = header.getSampleRate();
	if (sampleRate == 0) {
		return 0.0;
	}

	// MP3 frames contain 1152 samples
	double durationMs = (1152.0 / sampleRate) * 1000.0;
	return durationMs;
}
