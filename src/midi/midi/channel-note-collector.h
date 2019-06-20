#ifndef CHANNEL_NOTE_COLLECTOR_H
#define CHANNEL_NOTE_COLLECTOR_H

#include <vector>
#include <functional>
#include "midi-format.h"
#include "eventreceiver.h"

namespace midi
{
	class ChannelNoteCollector : public EventReceiver
	{
		Channel channel;
		Instrument instrument;
		Time start;
		std::vector<NOTE> active_notes;
		std::function<void(const NOTE&)> note_receiver;

	public:
		ChannelNoteCollector(Channel channel, std::function<void(const NOTE&)> note_receiver)
			: channel(channel), instrument(0), note_receiver(note_receiver)
		{
		}

		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override;
		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override;
		void program_change(Duration dt, Channel channel, Instrument program) override;
		void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override;
		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override;
	};
}

#endif