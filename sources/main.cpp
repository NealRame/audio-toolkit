#include <iostream>

#include <audio/format>
#include <audio/frame>
#include <audio/sequence>

using namespace com::nealrame;

const unsigned int CHANNEL_COUNT = 2;
const unsigned int SAMPLE_RATE   = 44100;

int main(int argc, char **argv) {
	char data[CHANNEL_COUNT*SAMPLE_RATE*sizeof(float)];
	utils::buffer buf(data, sizeof(data));
	audio::sequence seq(audio::format(CHANNEL_COUNT, SAMPLE_RATE), buf);

	auto first = seq.begin();

	for (audio::frame frame: seq) {
		for (float &sample: frame) {
			sample = 0;
		}
	}

	for (auto seq_it = first, seq_end = seq.end();
		seq_it != seq_end; ++seq_it) {

		std::cout << (seq_it - first) << ": ";

		audio::frame frame = *seq_it;
		for (auto frame_it = frame.begin(), frame_end = frame.end();
			frame_it != frame_end; ++frame_it) {
			std::cout << *frame_it << ' ';
		}

		std::cout << std::endl;
	}

	return 0;
}
