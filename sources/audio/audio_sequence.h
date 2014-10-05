/// audio_buffer.h
/// 
/// Created on: April 17, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#pragma once

#include <vector>
#include <iostream>

#include <boost/iterator/iterator_facade.hpp>

#include <utils/pimpl>
#include <audio/format>

namespace com {
namespace nealrame {
namespace audio {
/// class com::nealrame::audio::sequence
/// ====================================
class sequence {
public:
	class frame {
	public:
		using iterator = float *;
		using const_iterator = const iterator;
		using reference = float &;
		using const_reference = const reference;
		using size_type = format::size_type;

	public:
		frame() = delete;
		frame(const frame &);
		frame & operator=(const frame &);

	public:
		format::size_type channel_count() const;

		reference at(format::size_type channel);
		reference operator[](format::size_type channel)
		{ return at(channel); }

		const_reference at(format::size_type channel) const
		{ return const_cast<frame *>(this)->at(channel); }
		const_reference operator[](format::size_type channel) const
		{ return at(channel); }

	public:
		iterator begin();
		iterator end();

		const_iterator begin() const
		{ return const_cast<frame *>(this)->begin(); }
		const_iterator end() const
		{ return const_cast<frame *>(this)->end(); }

		const_iterator cbegin() const
		{ return const_cast<frame *>(this)->begin(); }
		const_iterator cend() const
		{ return const_cast<frame *>(this)->end(); }

	private:
		friend class sequence;
		friend std::ostream & operator<<(std::ostream &, const frame &);
		frame(iterator, iterator);
		iterator first_, last_;
	};

public:
	template <typename FrameType> class base_frame_iterator;
	using frame_iterator = base_frame_iterator<frame>;
	using const_frame_iterator = base_frame_iterator<const frame>;

public:
	/// Constructs an empty audio `sequence` with a given format.
	///
	/// *Parameters:*
	/// - `format`
	///    The required audio format.
	sequence(const class format &format) noexcept;

	/// Constructs an audio `sequence` containing the specifided count of audio
	/// frames. Frame are left un-initialized.
	///
	/// *Parameters:*
	/// - `frame_count`
	///   The requested count of frames.
	sequence(const class format &format, format::size_type frame_count);

	/// Constructs an audio `sequence` containing the required count of frames
	/// so that its duration reach the requested one. Frames are left
	/// un-initialized.
	sequence(const class format &format, double duration);

	/// Copy constructor.
	sequence(const sequence &);

	/// Move constructor.
	sequence(sequence &&) noexcept;

	/// destructor
	virtual ~sequence();

public:
	/// Copy operator.
	///
	/// *Parameters:*
	/// - `rhs`
	///   The `sequence` to be assigned from.
	sequence & operator=(const sequence &rhs);

	/// Move operator.
	///
	/// *Parameters:*
	/// - `rhs`
	///   The `sequence` to be assigned from.
	sequence & operator=(sequence &&) noexcept;

public:
	/// Swaps this `sequence` and the one specified.
	///
	/// *Parameters:*
	/// - `rhs`
	///   The `sequence` to be swaped with.
	void swap(sequence &rhs) noexcept;

public:
	/// Returns a reference on the `format` object of this `sequence`.
	class format & format() noexcept;

	/// Returns a constant reference on the `format` object of this 
	/// `sequence`.
	const class format & format() const noexcept
	{ return const_cast<sequence *>(this)->format(); }

	/// Returns the duration of this `sequence`.
	double duration() const noexcept;

	/// Returns the frame count of this `sequence`.
	format::size_type frame_count() const noexcept;

	/// Returns the frames capacity of this `sequence`.
	/// The `sequence` capacity is the maximum count of frames a `sequence`
	/// can contain before the need of a memory allocation.
	format::size_type capacity() const noexcept;

public:
	/// Returns a reference to the audio frame at the given index.
	/// 
	/// *Parameters:*
	/// - `idx`
	///   The index of the requested audio frame.
	frame at(format::size_type idx);
	frame operator[](format::size_type idx)
	{ return at(idx); }

	/// Returns a constant reference to the audio frame at the given index.
	/// 
	/// *Parameters:*
	/// - `idx`
	///   The index of the requested audio frame.
	const frame at(format::size_type idx) const;
	const frame operator[](format::size_type idx) const
	{ return at(idx); }

	/// Returns a pointer to the memory area starting at the specified
	/// frame.
	///
	/// *Parameters:*
	/// - `index`
	///   The index of the frame from which you wish to access the raw
	///   data sequence
	float * data(format::size_type index) noexcept;

	/// Returns a pointer to the constant memory area starting at the
	/// specified frame.
	///
	/// *Parameters:*
	/// - `index`
	///   The index of the frame from which you wish to access the raw
	///   data sequence
	const float * data(format::size_type index) const noexcept;

	/// Copy the specified count of frames into the given output
	/// interleaved raw sequence.
	///
	/// *Parameters:*
	/// - `pcm`
	///   The output interleaved raw sequence.
	/// - `frame_count`
	///   The requested count of frames to be copied.
	format::size_type copy(
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
	format::size_type copy(
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
	void append(const float *pcm, format::size_type frame_count);
	
	/// Append the given deinterleaved raw sequence to this `sequence`.
	///
	/// *Parameters:*
	/// - `pcm`
	///   A (**float) where the first index is the channel, and the second
	///   is the sample index.
	void append(const float * const *pcm, format::size_type frame_count);
	
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
	void append(const frame &);
	
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
	void append(const sequence &other) throw(error);

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
	void reserve(double duration);
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
	void reserve(format::size_type frame_count);

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
	frame_iterator set_duration(double duration);

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
	frame_iterator set_frame_count(format::size_type frame_count);

public:
	/// Returns a `frame_iterator` on the first frame of this `sequence`.
	frame_iterator begin();

	/// Returns a `const_frame_iterator` on the first audio frame of this
	/// `sequence`.
	const_frame_iterator begin() const;

	/// Returns a `const_frame_iterator` on the first audio frame of this
	/// `sequence`.
	const_frame_iterator cbegin() const;

	/// Returns a `frame_iterator` on the frame following the last audio 
	/// frame of this `sequence`.
	frame_iterator end();

	/// Returns a `const_frame_iterator` on the frame following the last
	/// audio frame of this `sequence`.
	const_frame_iterator end() const;

	/// Returns a `const_frame_iterator` on the frame following the last
	/// audio frame of this `sequence`.
	const_frame_iterator cend() const;

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
		base_frame_iterator() :
			ptr_{nullptr}, channel_count_(0)
		{ }

		/// Constructor.
		/// Constructs a `base_frame_iterator` from a given `frame`.
		/// 
		/// *Paramaters:*
		/// - `frame`
		///   A constant reference compatible `base_frame_iterator`.
		template <typename OTHER_FRAME_TYPE>
		base_frame_iterator(
			OTHER_FRAME_TYPE frame,
			typename std::enable_if<
				std::is_convertible<
					OTHER_FRAME_TYPE, 
					FRAME_TYPE
				>::value,
				enabler
			>::type = enabler()) :
			ptr_(frame.begin()), 
			channel_count_(frame.channel_count())
		{ }

		/// Constructor.
		/// Constructs a `base_frame_iterator` copying an other one.
		///
		/// *Paramaters:*
		/// - `other`
		///   A constant reference compatible `base_frame_iterator`.
		template <typename OTHER_FRAME_TYPE>
		base_frame_iterator(
			const base_frame_iterator<OTHER_FRAME_TYPE> &other,
			typename std::enable_if<
				std::is_convertible<
					OTHER_FRAME_TYPE,
					FRAME_TYPE
				>::value,
				enabler
			>::type = enabler()) :
			ptr_(other.ptr_),
			channel_count_(other.channel_count_) 
		{ }

	private:
		template <class> friend class base_frame_iterator;
		friend class com::nealrame::audio::sequence;
		friend boost::iterator_core_access;

	private:
		typename FRAME_TYPE::iterator ptr_;
		format::size_type channel_count_;

	private:
		FRAME_TYPE dereference() const
		{ return FRAME_TYPE(ptr_, ptr_ + channel_count_); }
		template <typename OTHER_FRAME_TYPE>
		bool equal (base_frame_iterator<OTHER_FRAME_TYPE> const &rhs) const
		{ return ptr_ == rhs.ptr_; }
		ptrdiff_t distance_to(base_frame_iterator const &rhs) const
		{ return (rhs.ptr_ - ptr_)/ptrdiff_t(channel_count_); }
		void advance(ptrdiff_t n)
		{ ptr_ += n*ptrdiff_t(channel_count_); }
		void increment()
		{ advance( 1); }
		void decrement()
		{ advance(-1); }
	};

private:
	PIMPL;
};

std::ostream & operator<<(std::ostream &, const sequence::frame &);

} // namespace audio
} // namespace nealrame
} // namespace com
