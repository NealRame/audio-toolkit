/// audio_coder.h
/// 
/// Created on: April 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_CODER_H_
#define AUDIO_CODER_H_

#include <fstream>
#include <memory>
#include <string>

#include <audio/error>

namespace com {
namespace nealrame {
namespace audio {
class buffer;
namespace codec {
class coder {
public:
	/// ### Encoding quality.
	/// The enum class `quality` defines four choice of quality sorted from
	/// best quality to worst quality or in other words from slowest to
	/// fastest algorithm :
	///
	/// - `coder::quality::Best`:
	///   best quality, encoding speed does not matter,
	/// - `coder::quality::Good`:
	///   good quality but not too slow,
	/// - `coder::quality::Acceptable`
	///   fast encoding speed with acceptable quality,
	/// - `coder::quality::Fastest`:
	///   encode as fast as possible, quality does not matter.
	enum class quality {
		Best, Good, Acceptable, Fastest,
		Default = Good
	};
public:
	/// ### Default constructor
	/// Initialize a coder with default quality.
	/// See section **Encoding quality**.
	coder ();
	
	/// Build a coder with the given quality.
	///
	/// *Parameters:*
	/// - `quality`
	///   The requested quality.
	coder (quality);
	
	virtual ~coder () {}

public:
	/// Return this `coder` quality.
	quality quality() const;
	
	/// Set this `coder` quality to the given one.
	/// See section **Encoding quality** for more details on `quality`.
	void set_quality(enum quality);

	/// Encode the given buffer to the given filename.
	/// See `buffer` documentation for more details about `buffer`.
	virtual void encode(const std::string &, const buffer &) const throw(error);
	
	/// Encode the given buffer to the given output stream.
	/// See `buffer` documentation for more details about `buffer`.
	virtual void encode(std::ofstream &, const buffer &) const throw(error) = 0;

private:
	enum quality quality_;
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_CODER_H_ */