#include "MP3.hpp"

#include <stdexcept>

MP3::MP3() : frames() {
}

MP3::~MP3() {
	clear();
}

uint32_t MP3::getFrameCount() const {
	return static_cast<uint32_t>(frames.size());
}

MP3Frame& MP3::getFrame(uint32_t index) {
	if (index >= frames.size()) {
		throw std::out_of_range("Frame index out of range");
	}
	return frames[index];
}

const MP3Frame& MP3::getFrame(uint32_t index) const {
	if (index >= frames.size()) {
		throw std::out_of_range("Frame index out of range");
	}
	return frames[index];
}

void MP3::addFrame(const MP3Frame& frame) {
	frames.push_back(frame);
}

bool MP3::removeFrame(uint32_t index) {
	if (index >= frames.size()) {
		return false;
	}
	frames.erase(frames.begin() + index);
	return true;
}

void MP3::clear() {
	frames.clear();
}

bool MP3::isEmpty() const {
	return frames.empty();
}

double MP3::getTotalDurationMs() const {
	double totalDuration = 0.0;
	for (const auto& frame : frames) {
		totalDuration += frame.getFrameDurationMs();
	}
	return totalDuration;
}

uint64_t MP3::getTotalAudioSize() const {
	uint64_t totalSize = 0;
	for (const auto& frame : frames) {
		totalSize += frame.getTotalFrameSize();
	}
	return totalSize;
}

const std::vector<MP3Frame>& MP3::getFrames() const {
	return frames;
}

std::vector<MP3Frame>& MP3::getFrames() {
	return frames;
}
