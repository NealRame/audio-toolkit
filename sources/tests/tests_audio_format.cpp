#include <gtest/gtest.h>

#include <audio/error>
#include <audio/format>

using namespace com::nealrame;

class audioFormatTest : public testing::Test {
protected:
	audioFormatTest() : 
		fmt_{ 2, 44100 },
		rates{16000, 22050, 44100, 48000, 96000},
		channels{1, 2, 3, 6, 8} {
	}
	com::nealrame::audio::format fmt_;
	std::vector<unsigned int> rates;
	std::vector<unsigned int> channels;
};

TEST_F(audioFormatTest, Constructor) {
	EXPECT_EQ(    2, fmt_.channel_count());
	EXPECT_EQ(44100, fmt_.sample_rate());
}

TEST_F(audioFormatTest, BadChannelCountValueConstructor) {
	try {
		audio::format fmt{ 0, 8000 };
		ADD_FAILURE_AT(__FILE__, __LINE__)
			<< "Should throw an audio::error exception";
	} catch (audio::error err) {
		EXPECT_EQ(audio::error::FormatBadChannelCountValue, err.status);
	}
}

TEST_F(audioFormatTest, BadSampleRateValueConstructor) {
	try {
		audio::format fmt{ 1, 0 };
		ADD_FAILURE_AT(__FILE__, __LINE__)
			<< "Should throw an audio::error exception";
	} catch (audio::error err) {
		EXPECT_EQ(audio::error::FormatBadSampleRateValue, err.status);
	}
}

TEST_F(audioFormatTest, BadByteDepthValueConstructor) {
	try {
		audio::format fmt{ 1, 8000 };
		ADD_FAILURE_AT(__FILE__, __LINE__)
			<< "Should throw an audio::error exception";
	} catch (audio::error err) {
		EXPECT_EQ(audio::error::FormatBadBitDepthValue, err.status);
	}
}

TEST_F(audioFormatTest, SetChannelCountValue) {
	for (unsigned int channel_count : channels) {
		fmt_.set_channel_count(channel_count);
		EXPECT_EQ(channel_count, fmt_.channel_count());
	}
}

TEST_F(audioFormatTest, SetBadChannelCountValue) {
	try {
		fmt_.set_channel_count(0);
		ADD_FAILURE_AT(__FILE__, __LINE__)
			<< "Should throw an audio::error exception";
	} catch (audio::error err) {
		EXPECT_EQ(audio::error::FormatBadChannelCountValue, err.status);
	}

}

TEST_F(audioFormatTest, SetSampleRateValue) {
	for (unsigned int sample_rate : rates) {
		fmt_.set_sample_rate(sample_rate);
		EXPECT_EQ(sample_rate, fmt_.sample_rate());
	}
}

TEST_F(audioFormatTest, SetBadSampleRateValue) {
	try {
		fmt_.set_sample_rate(0);
		ADD_FAILURE_AT(__FILE__, __LINE__)
			<< "Should throw an audio::error exception";
	} catch (audio::error err) {
		EXPECT_EQ(audio::error::FormatBadSampleRateValue, err.status);
	}

}

TEST_F(audioFormatTest, Size) {
	for (unsigned int channel_count : channels) {
		for (unsigned int sample_rate : rates) {
			// expected size of sample_rate frames or 1s.
			size_t expected_size = channel_count*sample_rate*sizeof(float);
			audio::format fmt{channel_count, sample_rate};
			EXPECT_EQ(expected_size, fmt.size(sample_rate));
			EXPECT_EQ(expected_size, fmt.size(1.));
		}
	}
}

TEST_F(audioFormatTest, Duration) {
	for (unsigned int channel_count : channels) {
		for (unsigned int sample_rate : rates) {
			// exepected size for 1s
			size_t size = channel_count*sample_rate*sizeof(float);
			audio::format fmt{channel_count, sample_rate};
			EXPECT_DOUBLE_EQ(1., fmt.duration(sample_rate));
			EXPECT_DOUBLE_EQ(1., fmt.duration(size));
		}
	}
}

TEST_F(audioFormatTest, FrameCount) {
	for (unsigned int channel_count : channels) {
		for (unsigned int sample_rate : rates) {
			// exepected size for sample_rate frames
			size_t size = channel_count*sample_rate*sizeof(float);
			audio::format fmt{channel_count, sample_rate};
			EXPECT_DOUBLE_EQ(sample_rate, fmt.frame_count(size));
			EXPECT_DOUBLE_EQ(sample_rate, fmt.frame_count(1.));
		}
	}
}