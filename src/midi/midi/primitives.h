#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <cstdint>
#include "util/tagged.h"

namespace midi
{
	struct Time;

	struct __declspec(empty_bases)Channel : show_value<Channel, int>, equality<Channel>, tagged<uint8_t, Channel> { using tagged::tagged; };

	struct __declspec(empty_bases)Instrument : show_value<Instrument, int>, equality<Instrument>, tagged<uint8_t, Instrument> { using tagged::tagged; };

	struct __declspec(empty_bases)NoteNumber : show_value<NoteNumber, int>, ordered<NoteNumber>, tagged<uint8_t, NoteNumber> { using tagged::tagged; };

	struct __declspec(empty_bases)Duration : show_value<Duration, uint64_t>, ordered<Duration>, tagged<uint64_t, Duration>
	{
		using tagged::tagged;

		Duration operator +(const Duration&) const;
		Duration operator -(const Duration&) const;
		Time operator +(const Time&) const;
		Duration& operator +=(const Duration&);
		Duration& operator -=(const Duration&);
	};

	struct __declspec(empty_bases)Time : show_value<Time, uint64_t>, ordered<Time>, tagged<uint64_t, Time> 
	{ 
		using tagged::tagged;

		Duration operator -(const Time&) const;
		Time operator +(const Time&) const;
		Time operator +(const Duration&) const;
		Time& operator +=(const Duration&);
	};

}

#endif