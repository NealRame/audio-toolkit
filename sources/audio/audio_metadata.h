// audio_metadata.h
//
// Created on: June 03, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_METADATA_H_
#define AUDIO_METADATA_H_

#include <utils/buffer>

#include <functional>
#include <string>
#include <unordered_map>

namespace com {
namespace nealrame {
namespace audio {

class metadata {
public:
	enum tag_multiplicity {
		Singleton,
		Multiple
	};

	struct tag {
		std::string name;
		tag_multiplicity multiplicity;
	};

	/// The following tags are borrowed from the Ogg Vorbis Field 
	/// Recommendations.
	/// See http://age.hobba.nl/audio/mirroredpages/ogg-tagging.html for more
	/// informations.

	/// Singleton tags:
	/// * `metadata::Album`
	///   If appropriate, an album name.
	/// 
	/// * `metadata::Artist`
	///   It is not a replacement for the `Ensemble` and `Performer` tags,
	///   but typically will summarize them.
	///
	/// * `metadata::Publisher`
	///   Who publishes the disc the track came from.
	/// 
	/// * `metadata::Copyright`
	///   Who holds copyright to the track or disc the track is on.
	/// 
	/// * `metadata::DiskNumber`
	///   If part of a multi-disc album, put the disc number here.
	/// 
	/// * `metadata::ISRC`
	///   This number lets you order a CD over the phone from a record
	///   shop.
	/// 
	/// * `metadata::EAN_UPN`
	///   There may be a barcode on the CD; it is most likely an EAN or UPN
	///   (Universal Product Number).
	///
	/// * `metadata::Label`
	///   The record label or imprint on the disc.
	///
	/// * `metadata::LabelInfo`
	///   Record labels often put the catalog number of the source media
	///   somewhere on the packaging. Use this tag to record it.
	///
	/// * `metadata::License`
	///   The license, or URL for the license the track is under. For
	///   instance, the Open Audio license.
	///
	/// * `metadata::Opus`
	///   The number of the work; eg, Opus 10, BVW 81, K6.
	///
	/// * `metadata::SourceMedia`
	///   The recording media the track came from. eg, CD, Cassette,
	///   Radio Broadcast, LP, CD Single.
	///
	/// * `metadata::Title`
	///   "the work", whether a symphony, or a pop song.
	///
	/// * `metadata::TrackNumber`
	///   The track number on the CD.
	///
	/// * `metadata::Version`
	///   Make sure you don't put `Date` or `Location` information in this
	///   tag. "live", "acoustic", "Radio Edit" "12 inch remix" might be
	///   typical values of this tag.
	///
	/// * `metadata::EncodedBy`
	///   The person who encoded the Ogg file. May include contact
	///   information such as email address and phone number.
	///
	/// * `metadata::Encoding`
	///   Put the settings you used to encode the Ogg file here. This tag
	///   is NOT expected to be stored or returned by cddb type databases.
	///   It includes information about the quality setting, bit rate, and
	///   bitrate management settings used to encode the Ogg.
	///   It is also used for information about which encoding software was
	///   used to do the encoding.
	///
	/// Multiple tags
	/// * `metadata::Composer`
	/// * `metadata::Arranger`
	/// * `metadata::Lyricist`
	/// * `metadata::Author`
	/// * `metadata::Conductor`
	/// * `metadata::Performer`
	/// * `metadata::Ensemble`
	/// * `metadata::Part`
	/// * `metadata::PartNumber`
	/// * `metadata::Genre`
	/// * `metadata::Date`
	/// * `metadata::Location`
	/// * `metadata::Comment`
	/// * `metadata::BlockPicture`

	static const tag Album;
	static const tag Artist;
	static const tag Publisher;
	static const tag Copyright;
	static const tag DiskNumber;
	static const tag ISRC;
	static const tag EAN_UPN;
	static const tag Label;
	static const tag LabelInfo;
	static const tag License;
	static const tag Opus;
	static const tag SourceMedia;
	static const tag Title;
	static const tag TrackNumber;
	static const tag Version;
	static const tag EncodedBy;
	static const tag Encoding;
	static const tag Composer;
	static const tag Arranger;
	static const tag Lyricist;
	static const tag Author;
	static const tag Conductor;
	static const tag Performer;
	static const tag Ensemble;
	static const tag Part;
	static const tag PartNumber;
	static const tag Genre;
	static const tag Date;
	static const tag Location;
	static const tag Comment;
	static const tag BlockPicture;

private:
	std::unordered_multimap<tag, utils::buffer, std::function<void (tag const &)>> tags_;
};

} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_METADATA_H_ */