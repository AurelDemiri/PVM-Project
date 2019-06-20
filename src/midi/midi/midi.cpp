#include "midi.h"
#include "io/endianness.h"
#include "io/read.h"
#include "io/vli.h"

void midi::read_chunk_header(std::istream& in, midi::CHUNK_HEADER* ch)
{
	io::read_to(in, ch);
	io::switch_endianness(&ch->size);
}

std::string midi::header_id(const midi::CHUNK_HEADER& ch)
{
	return std::string(ch.id, sizeof(ch.id));
}

void midi::read_mthd(std::istream& in, midi::MTHD* mthd)
{
	io::read_to(in, mthd);
	io::switch_endianness(&mthd->header.size);
	io::switch_endianness(&mthd->type);
	io::switch_endianness(&mthd->ntracks);
	io::switch_endianness(&mthd->division);
}

void midi::read_mtrk(std::istream& in, midi::EventReceiver& er)
{
	char id[4];
	io::read_to(in, &id);
	CHECK(strcmp(id, "MTrk") != 0);

	uint32_t chunk_length = io::read<uint32_t>(in);
	io::switch_endianness(&chunk_length);

	uint8_t identifier;
	while (true)
	{
		midi::Duration delta_time = midi::Duration(io::read_variable_length_integer(in));

		if (!is_running_status(static_cast<uint8_t>(in.peek())))
			identifier = io::read<uint8_t>(in);

		if (is_meta_event(identifier))
		{
			uint8_t type = io::read<uint8_t>(in);
			uint64_t data_size = io::read_variable_length_integer(in);
			auto data = io::read_array<uint8_t>(in, data_size);
			
			er.meta(delta_time, type, std::move(data), data_size);
			
			// END OF TRACK
			if (type == 0x2F)
				break;
		}
		else if (is_sysex_event(identifier))
		{
			uint64_t data_size = io::read_variable_length_integer(in);
			auto data = io::read_array<uint8_t>(in, data_size);

			er.sysex(delta_time, std::move(data), data_size);
		}
		else if (is_midi_event(identifier))
		{
			// identifier == status for midi events
			midi::Channel channel = extract_midi_event_channel(identifier);
			uint8_t midi_event_type = extract_midi_event_type(identifier);

			if (is_note_off(midi_event_type))
			{
				midi::NoteNumber note = midi::NoteNumber(io::read<uint8_t>(in));
				uint8_t velocity = io::read<uint8_t>(in);

				er.note_off(delta_time, channel, note, velocity);
			}
			else if (is_note_on(midi_event_type))
			{
				midi::NoteNumber note = midi::NoteNumber(io::read<uint8_t>(in));
				uint8_t velocity = io::read<uint8_t>(in);

				er.note_on(delta_time, channel, note, velocity);
			}
			else if (is_polyphonic_key_pressure(midi_event_type))
			{
				midi::NoteNumber note = midi::NoteNumber(io::read<uint8_t>(in));
				uint8_t pressure = io::read<uint8_t>(in);

				er.polyphonic_key_pressure(delta_time, channel, note, pressure);
			}
			else if (is_control_change(midi_event_type))
			{
				uint8_t controller = io::read<uint8_t>(in);
				uint8_t value = io::read<uint8_t>(in);

				er.control_change(delta_time, channel, controller, value);
			}
			else if (is_program_change(midi_event_type))
			{
				midi::Instrument program = midi::Instrument(io::read<uint8_t>(in));

				er.program_change(delta_time, channel, program);
			}
			else if (is_channel_pressure(midi_event_type))
			{
				uint8_t pressure = io::read<uint8_t>(in);

				er.channel_pressure(delta_time, channel, pressure);
			}
			else if (is_pitch_wheel_change(midi_event_type))
			{
				uint8_t lower_bits = io::read<uint8_t>(in);
				uint8_t upper_bits = io::read<uint8_t>(in);
				uint16_t wheel = (upper_bits << 7) | lower_bits;

				er.pitch_wheel_change(delta_time, channel, wheel);
			}
		}
	}
}

std::vector<midi::NOTE> midi::read_notes(std::istream& in)
{
	std::vector<midi::NOTE> notes;

	midi::MTHD mthd;
	midi::read_mthd(in, &mthd);

	for (uint16_t i = 0; i < mthd.ntracks; i++)
	{
		midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });
		midi::read_mtrk(in, collector);
	}

	return notes;
}

bool midi::is_sysex_event(uint8_t byte)
{
	return byte == 0xF0 || byte == 0xF7;
}

bool midi::is_meta_event(uint8_t byte)
{
	return byte == 0xFF;
}

bool midi::is_midi_event(uint8_t byte)
{
	// TODO: Make a midi_event::MIN and MAX value
	uint8_t event_type = extract_midi_event_type(byte);
	return event_type >= midi::midi_event::note_off && event_type <= midi::midi_event::pitch_wheel_change;
}

bool midi::is_running_status(uint8_t byte)
{
	return !(byte >> 7);
}

uint8_t midi::extract_midi_event_type(uint8_t status)
{
	return (status >> 4);
}

midi::Channel midi::extract_midi_event_channel(uint8_t status)
{
	return midi::Channel((status & 15));
}

bool midi::is_note_off(uint8_t status)
{
	return status == midi::midi_event::note_off;
}

bool midi::is_note_on(uint8_t status)
{
	return status == midi::midi_event::note_on;
}

bool midi::is_polyphonic_key_pressure(uint8_t status)
{
	return status == midi::midi_event::polyphonic_key_pressure;
}

bool midi::is_control_change(uint8_t status)
{
	return status == midi::midi_event::control_change;
}

bool midi::is_program_change(uint8_t status)
{
	return status == midi::midi_event::program_change;
}

bool midi::is_channel_pressure(uint8_t status)
{
	return status == midi::midi_event::channel_pressure;
}

bool midi::is_pitch_wheel_change(uint8_t status)
{
	return status == midi::midi_event::pitch_wheel_change;
}