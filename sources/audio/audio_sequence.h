/// audio_buffer.h
/// 
/// Created on: April 17, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_BUFFER_H_
#define AUDIO_BUFFER_H_

#include <vector>
#include <iostream>

#include <boost/iterator/iterator_facade.hpp>

#include <audio/format>

namespace com {
namespace nealrame {
namespace audio {
/// class com::nealrame::audio::sequence
/// ====================================
class sequence {
public:
	class frame;
	class const_frame;
	template <typename FrameType> class base_frame_iterator;
	using frame_iterator = base_frame_iterator<frame>;
	using const_frame_iterator = base_frame_iterator<const_frame>;
public:
	/// Constructs an empty audio `sequence` with a given format.
	///
	/// *Parameters:*
	/// - `format`
	///    The required audio format.
	sequence (const class format &format) noexcept;

	/// Constructs an audio `sequence` containing the specifided count of
	/// audio frame.
	/// Frame are left un-initialized.
	///
	/// *Parameters:*
	/// - `frame_count`
	///   The requested count of frames.
	sequence (const class format &format, format::size_type frame_count);

	/// Constructs an audio `sequence` containing the required count of
	/// frames soi that its duration reach the requested one.
	sequence (const class format &format, double duration);

	/// Copy constructor.
	sequence (const sequence &);

	/// Move constructor.
	sequence (sequence &&) noexcept;

public:
	/// Copy operator.
	///
	/// *Parameters:*
	/// - `rhs`
	///   The `sequence` to be assigned from.
	sequence & operator= (const sequence &rhs);

	/// Move operator.
	///
	/// *Parameters:*
	/// - `rhs`
	///   The `sequence` to be assigned from.
	sequence & operator= (sequence &&) noexcept;

public:
	/// Swaps this `sequence` and the one specified.
	///
	/// *Parameters:*
	/// - `rhs`
	///   The `sequence` to be swaped with.
	void swap (sequence &rhs) noexcept;

public:
	/// Returns a reference on the `format` object of this `sequence`.
	class format & format () noexcept
	{ return format_; }

	/// Returns a constant reference on the `format` object of this 
	/// `sequence`.
	const class format & format () const noexcept
	{ return const_cast<sequence *>(this)->format(); }

	/// Returns the duration of this `sequence`.
	double duration () const noexcept;

	/// Returns the frame count of this `sequence`.
	format::size_type frame_count () const noexcept;

	/// Returns the frames capacity of this `sequence`.
	/// The `sequence` capacity is the maximum count of frames a `sequence`
	/// can contain before the need of a memory allocation.
	format::size_type capacity () const noexcept;

public:
	/// Returns a reference to the audio frame at the given index.
	/// 
	/// *Parameters:*
	/// - `idx`
	///   The index of the requested audio frame.
	frame at (format::size_type idx);
	frame operator[] (format::size_type idx)
	{ return at(idx); }

	/// Returns a constant reference to the audio frame at the given index.
	/// 
	/// *Parameters:*
	/// - `idx`
	///   The index of the requested audio frame.
	const_frame at (format::size_type idx) const;
	const_frame operator[] (format::size_type idx) const
	{ return at(idx); }

	/// Returns a pointer to the memory area starting at the specified
	/// frame.
	///
	/// *Parameters:*
	/// - `index`
	///   The index of the frame from which you wish to access the raw
	///   data sequence
	float * data (format::size_type index) noexcept;

	/// Returns a pointer to the constant memory area starting at the
	/// specified frame.
	///
	/// *Parameters:*
	/// - `index`
	///   The index of the frame from which you wish to access the raw
	///   data sequence
	const float * data (format::size_type index) const noexcept;

	/// Copy the specified count of frames into the given output
	/// interleaved raw sequence.
	///
	/// *Parameters:*
	/// - `pcm`
	///   The output interleaved raw sequence.
	/// - `frame_count`
	///   The requested count of frames to be copied.
	format::size_type copy (
			float *pcm,
			format::size_type frame_count,
			format::size_type offset = 0) const;

	/// Copy the specified count of frames into the given output
	/// deinterleaved raw sequence.
	///
	/// *Parameters:*
	/// - `pcm`
	///   The output deinterleaved raw sequence. A (**float) where the 
	///   first index is the channel, and the second is the sample index.
	/// - `frame_count`
	///   The requested count of frames to be copied.
	format::size_type copy (
			float **pcm, 
			format::size_type frame_count, 
			format::size_type offset = 0) const;

public:
	/// Append the given interleaved raw sequence to this `sequence`.
	///
	/// *Parameters:*
	/// - `pcm`
	///   An array of interleaved samples.
	/// - `frame_count`
	///   The count of frames to be appended.
	void append (const float *pcm, format::size_type frame_count);
	
	/// Append the given deinterleaved raw sequence to this `sequence`.
	///
	/// *Parameters:*
	/// - `pcm`
	///   A (**float) where the first index is the channel, and the second
	///   is the sample index.
	void append (const float * const *pcm, format::size_type frame_count);
	
	/// Append the given `const_frame` to this `sequence`.
	///
	/// *Parameters:*
	/// - `frame`
	///   The frame to be appended.
	///
	/// *Exceptions:*
	/// - `error`
	///   If the count of channels of the given `const_frame` is different
	///   than the count of channel of this `sequence` an `error` 
	///   exeception with status `FormatMismatched` will be raised.
	void append (const_frame frame);
	
	/// Append the given `sequence` to this `sequence`.
	///
	/// If the required capacity to append the given `sequence`is greater
	/// than the current capacity of this `sequence`, all frames iterators
	/// or references are invalidated. Otherwise, no frales iterators or
	/// references are invalidated.
	///
	/// *Parameters:*
	/// - `other`
	///   A constant reference to a `sequence`.
	///
	/// *Exceptions:*
	/// - `error`
	///   If the given `sequence` format is different than the format of
	///   this `sequence` an `error` exeception with status 
	///   `FormatMismatched` will be raised.
	void append (const sequence &other) throw(error);

	/// Sets this audio `sequence`'s capacity so that it can contain enough
	/// audio frames for the specified duration.
	///
	/// If the requested capacity is greater than the current capacity,
	/// all frames iterators and references are invalidated. Otherwise, no
	/// frames iterators or references are invalidated.
	///
	/// *Parameters:*
	/// - `duration`
	///   The requested duration.
	void reserve (double duration);
	/// Sets the capacity of this audio `sequence` so that it can contain
	/// as many audio frames as specified.
	///
	/// If the requested capacity is greater than the current capacity,
	/// all frames iterators and references are invalidated. Otherwise, no
	/// frames iterators or references are invalidated.
	///
	/// *Parameters:$
	/// - `frame_count`
	///   The requested count of frames.
	void reserve (format::size_type frame_count);

	/// Sets count of audio frames of this `sequence` so that its duration
	/// reach the requested one.
	///
	/// If the required capacity is greater than the current capacity of 
	/// this `sequence`, all the audio frames iterators and references are
	/// invalidated. Otherwise, no audio frames iterator or references are
	/// invalidated.
	///
	/// If the requested count of audio frames is lower than the current 
	/// count, this `sequence` is reduced to the requested count.
	///
	/// *Parameter:*
	/// - `duration`
	///   The requested duration.
	frame_iterator set_duration (double duration);

	/// Sets the count of audio frames of this sequence to the requested 
	/// one.
	///
	/// If the required capacity is greater than the current capacity of
	/// this `sequence`, all the audio frames iterators and references are
	/// invalidated. Otherwise, no audio frames iterator or references are
	/// invalidated.
	///
	/// If the requested count of audio frames is lower than the current
	/// count, this `sequence` is reduced to the requested count.
	///
	/// *Parameter:*
	/// - `frame_count`
	///   The requested count of frames.
	frame_iterator set_frame_count (format::size_type frame_count);

public:
	/// Returns a `frame_iterator` on the first frame of this `sequence`.
	frame_iterator begin ()
	{ return frame_iterator(frames_.begin(), format_.channel_count()); }

	/// Returns a `const_frame_iterator` on the first audio frame of this
	/// `sequence`.
	const_frame_iterator begin () const
	{ return const_cast<sequence *>(this)->begin(); }

	/// Returns a `const_frame_iterator` on the first audio frame of this
	/// `sequence`.
	const_frame_iterator cbegin () const
	{ return const_cast<sequence *>(this)->begin(); }

	/// Returns a `frame_iterator` on the frame following the last audio 
	/// frame of this `sequence`.
	frame_iterator end ()
	{ return begin() + frame_count(); }

	/// Returns a `const_frame_iterator` on the frame following the last
	/// audio frame of this `sequence`.
	const_frame_iterator end () const
	{ return const_cast<sequence *>(this)->end(); }

	/// Returns a `const_frame_iterator` on the frame following the last
	/// audio frame of this `sequence`.
	const_frame_iterator cend () const
	{ return const_cast<sequence *>(this)->end(); }

public:
	/// class com::nealrame::audio::sequence::base_frame_iterator
	/// =======================================================
	template <typename FRAME_TYPE>
	class base_frame_iterator :
		public boost::iterator_facade<
			base_frame_iterator<FRAME_TYPE>, 
			FRAME_TYPE, 
			boost::random_access_traversal_tag, 
			FRAME_TYPE
		> {
	public:
		struct enabler {};
	public:
		/// Default constructor.
		base_frame_iterator () :
			channel_count_(0) {
		}

		/// Copy constructor.
		/// Constructs a `base_frame_iterator` copying an other one.
		///
		/// *Paramaters:*
		/// - `other`
		///   A constant reference compatible `base_frame_iterator`.
		template <typename OTHER_FRAME_TYPE>
		base_frame_iterator (
			const base_frame_iterator<OTHER_FRAME_TYPE> &other,
			typename std::enable_if<
				std::is_convertible<
					typename OTHER_FRAME_TYPE::reference,
					typename FRAME_TYPE::reference
				>::value,
				enabler
			>::type = enabler()) :
			ptr_(other.ptr_),
			channel_count_(other.channel_count_) {
		}
	private:
		template <class> friend class base_frame_iterator;
		friend class com::nealrame::audio::sequence;
		friend boost::iterator_core_access;
	private:
		typename FRAME_TYPE::iterator ptr_;
		format::size_type channel_count_;
	private:
		template <typename Iterator>
		base_frame_iterator (
			Iterator ptr,
			format::size_type channel_count,
			typename std::enable_if<
				std::is_convertible<
					typename Iterator::reference,
					typename FRAME_TYPE::reference
				>::value,
				enabler
			>::type = enabler()) :
			ptr_(ptr),
			channel_count_(channel_count) {
		}
		FRAME_TYPE dereference () const
		{ return FRAME_TYPE(ptr_, channel_count_); }
		template <typename OTHER_FRAME_TYPE>
		bool equal (base_frame_iterator<OTHER_FRAME_TYPE> const &rhs) const
		{ return ptr_ == rhs.ptr_; }
		ptrdiff_t distance_to (base_frame_iterator const &rhs) const
		{ return (rhs.ptr_ - ptr_)/ptrdiff_t(channel_count_); }
		void advance (ptrdiff_t n)
		{ ptr_ += n*ptrdiff_t(channel_count_); }
		void increment ()
		{ advance( 1); }
		void decrement ()
		{ advance(-1); }
	};
	/// class com::nealrame::audio::sequence::const_frame
	/// ===============================================
	class const_frame {
#if defined(DEBUG)
	friend std::ostream & operator<< (std::ostream &, sequence::const_frame);
#endif
	public:
		using iterator = std::vector<float>::const_iterator;
		using const_iterator = std::vector<float>::const_iterator;
		using reference = const float &;
		using size_type = format::size_type;
	public:
		struct enabler {};
		template <typename FrameType>
		const_frame (const FrameType &rhs,
			typename std::enable_if<
				std::is_convertible<
					typename FrameType::reference,
					reference
				>::value,
				enabler
			>::type = enabler()) :
			first_(rhs.first_),
			last_(rhs.last_) {
		}
	public:
		/// Returns the count of channel of this 
		/// `sequence::const_frame`.
		size_type channel_count () const
		{ return last_ - first_; }
	public:
		/// Returns a const reference to the sample at the specified 
		/// channel of this `sequence::const_frame`.
		///
		/// *Parameters:*
		/// - `channel`
		///   The request channel.
		const float & at (size_type channel) const
		{ return *(first_ + channel); }
		const float & operator[] (format::size_type channel) const
		{ return at(channel); }
		/// Returns an `iterator` to the constant first sample of this
		/// `sequence::const_frame`.
		iterator begin () const
		{ return first_; }
		/// Returns an `iterator` to the constant first sample of this
		/// `sequence::const_frame`.
		iterator cbegin () const
		{ return first_; }
		/// Returns an `iterator` to the constant sample following the
		/// last sample of this `sequence::const_frame`.
		iterator end () const
		{ return last_; }
		/// Returns an `iterator` to the constant sample following the
		/// last sample of this `sequence::const_frame`.
		iterator cend () const
		{ return last_; }
	private:
		const_frame (iterator first, size_type channel_count) :
			const_frame(first, first + channel_count) {
		}
		const_frame (iterator first, iterator last) :
			first_(first), last_(last) {
		}
	private:
		iterator first_;
		iterator last_;
		friend class com::nealrame::audio::sequence;
		friend class base_frame_iterator<const_frame>;
	};
	/// class com::nealrame::audio::sequence::frame
	/// =========================================
	class frame {
	public:
		using iterator = std::vector<float>::iterator;
		using const_iterator = std::vector<float>::const_iterator;
		using reference = float &;
		using size_type = format::size_type;
	public:
		/// Copy constructor.
		/// Initializes this `frame` with the given `frame`.
		/// After that this `frame` and the other one will refere to
		/// the same sequence of sample, that is to say, updates on one
		/// frame cause updates on the other.
		///
		/// *Paramaters*:
		/// - `rhs`
		///   The frame to be referred.
		frame (const frame &rhs) :
			first_(rhs.first_),
			last_(rhs.last_) {
		}

	public:
		/// Replaces the content of this `sequence::frame` with the
		/// content of the given `sequence::frame`.
		///
		/// *Parameters:$
		///  - `rhs`
		///    The `sequence::frame` to be assigned from.
		frame & operator= (const frame &rhs) {
			std::copy(rhs.first_, rhs.last_, first_);
			return *this;
		}
		/// Replaces the content of this `sequence::const_frame` with 
		/// the content of the given `sequence::const_frame`.
		///
		/// *Parameters:$
		///  - `rhs`
		///    The `sequence::const_frame` to be assigned from.
		frame & operator= (const const_frame &rhs) {
			std::copy(rhs.begin(), rhs.end(), first_);
			return *this;
		}
	public:
		/// Returns the count of channel of this `sequence::frame`.
		format::size_type channel_count () const
		{ return last_ - first_; }
	public:
		/// Returns a reference to the sample at the specified channel
		/// of this `sequence::frame`.
		///
		/// *Parameters:*
		/// - `channel`
		///   The request channel.
		float & at (format::size_type channel)
		{ return *(first_ + channel); }
		float & operator[] (format::size_type channel)
		{ return at(channel); }
		/// Returns a constant reference to the sample at the specified
		/// channel of this `sequence::frame`.
		///
		/// *Parameters:*
		/// - `channel`
		///   The request channel.
		float & at (format::size_type channel) const
		{ return const_cast<frame *>(this)->at(channel); }
		float & operator[] (format::size_type channel) const
		{ return const_cast<frame *>(this)->at(channel); }
		/// Returns an `iterator` to the first sample of this
		/// `sequence::frame`.
		iterator begin () 
		{ return first_; }
		/// Returns an `iterator` to the first constant sample of this
		/// `sequence::frame`.
		const_iterator begin () const
		{ return const_cast<frame *>(this)->begin(); }
		/// Returns an `iterator` to the first constant sample of this
		/// `sequence::frame`.
		const_iterator cbegin () const
		{ return const_cast<frame *>(this)->begin(); }
		/// Returns an `iterator` to the sample following the last
		/// sample of this `sequence::frame`.
		iterator end ()
		{ return last_; }
		/// Returns an `iterator` to the constant sample following the
		/// last sample of this `sequence::frame`.
		const_iterator end () const
		{ return const_cast<frame *>(this)->end(); }
		/// Returns an `iterator` to the constant sample following the
		/// last sample of this `sequence::frame`.
		const_iterator cend () const
		{ return const_cast<frame *>(this)->end(); }	
	private:
		frame (iterator first, format::size_type channel_count) :
			frame(first, first + channel_count) {
		}
		frame (iterator first, iterator last) :
			first_(first), last_(last) {
		}
	private:
		iterator first_;
		iterator last_;
		friend class base_frame_iterator<frame>;
		friend class com::nealrame::audio::sequence;
	};
private:
	class format format_;
	std::vector<float> frames_;
};
} // namespace audio
} // namespace nealrame
} // namespace com
#endif /* AUDIO_BUFFER_H_ */