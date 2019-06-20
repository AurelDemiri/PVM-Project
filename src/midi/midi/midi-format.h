#ifndef MIDI_FORMAT_H
#define MIDI_FORMAT_H

#include "primitives.h"

namespace midi
{
	// In this case we want traditional enums to convert to uint8_t too readily.
	// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#enum3-prefer-class-enums-over-plain-enums
	enum midi_event : uint8_t
	{
		note_off = 0x08,
		note_on = 0x09,
		polyphonic_key_pressure = 0x0A,
		control_change = 0x0B,
		program_change = 0x0C,
		channel_pressure = 0x0D,
		pitch_wheel_change = 0x0E
	};

	struct CHUNK_HEADER
	{
		char id[4];
		uint32_t size;
	};

#pragma pack(push, 1)
	struct MTHD
	{
		CHUNK_HEADER header;
		uint16_t type;
		uint16_t ntracks;
		uint16_t division;
	};
#pragma pack(pop)

	struct NOTE
	{
		NoteNumber note_number;
		Time start;
		Duration duration;
		uint8_t velocity;
		Instrument instrument;

		NOTE(NoteNumber note_number, Time start, Duration duration, uint8_t velocity, Instrument instrument)
			: note_number(note_number), start(start), duration(duration), velocity(velocity), instrument(instrument)
		{
		}

		bool operator ==(const NOTE& rhs) const
		{
			return note_number == rhs.note_number && start == rhs.start && duration == rhs.duration &&
				velocity == rhs.velocity && instrument == rhs.instrument;
		}

		friend std::ostream& operator <<(std::ostream& out, const NOTE& x)
		{
			return out << "Note(number=" << x.note_number << ",start=" << x.start << ",duration=" << x.duration << ",instrument=" << x.instrument << ")";
		}
	};
}

#endif