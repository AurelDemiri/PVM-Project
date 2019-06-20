#include <algorithm>
#include "channel-note-collector.h"

void midi::ChannelNoteCollector::meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	start += dt;
}

void midi::ChannelNoteCollector::sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	start += dt;
}

void midi::ChannelNoteCollector::note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	if (velocity == 0)
	{
		note_off(dt, channel, note, velocity);
	}
	else
	{
		if (channel == this->channel && active_notes.size() > 0 && active_notes[active_notes.size() - 1].note_number == note && active_notes[active_notes.size() - 1].velocity != 0)
			note_receiver(midi::NOTE(note, Time() + active_notes[active_notes.size() - 1].duration, dt, active_notes[active_notes.size() - 1].velocity, instrument));

		start += dt;
		active_notes.push_back(midi::NOTE(note, start, dt, velocity, instrument));
	}
}

void midi::ChannelNoteCollector::note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	start += dt;

	// We need note_off events to have 0 velocity to recognize them.
	active_notes.push_back(midi::NOTE(note, start, dt, 0, instrument));

	auto it = std::find_if(active_notes.rbegin(), active_notes.rend(), [&note](midi::NOTE a)->bool { return a.note_number == note && a.velocity != 0; });
	ptrdiff_t index = std::distance(active_notes.begin(), it.base()) - 1;

	Duration realdt;
	for (size_t i = index; i < active_notes.size(); i++)
	{
		if (i != index)
			realdt += active_notes[i].duration;
	}

	if (channel == this->channel)
		note_receiver(midi::NOTE(note, active_notes[index].start, realdt, active_notes[index].velocity, instrument));
}

void midi::ChannelNoteCollector::polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure)
{
	start += dt;
}

void midi::ChannelNoteCollector::control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value)
{
	start += dt;
}

void midi::ChannelNoteCollector::program_change(Duration dt, Channel channel, Instrument program)
{
	start += dt;

	if (channel == this->channel)
	{
		instrument = program;
	}
}

void midi::ChannelNoteCollector::channel_pressure(Duration dt, Channel channel, uint8_t pressure)
{
	start += dt;
}

void midi::ChannelNoteCollector::pitch_wheel_change(Duration dt, Channel channel, uint16_t value)
{
	start += dt;
}