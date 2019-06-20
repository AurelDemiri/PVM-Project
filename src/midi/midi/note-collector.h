#ifndef NOTE_COLLECTOR_H
#define NOTE_COLLECTOR_H

#include "channel-note-collector.h"
#include "event-multicaster.h"

namespace midi
{
	class NoteCollector : public EventReceiver
	{
		std::function<void(const NOTE&)> note_receiver;
		std::unique_ptr<EventMulticaster> multicaster;

	public:
		NoteCollector(std::function<void(const NOTE&)> note_receiver)
			: note_receiver(note_receiver)
		{
			std::vector<std::shared_ptr<ChannelNoteCollector>> collectors;

			for (uint8_t i = 0; i < 16; i++)
			{
				collectors.push_back(std::make_shared<ChannelNoteCollector>(ChannelNoteCollector(Channel(i), note_receiver)));
			}

			multicaster = std::make_unique<EventMulticaster>(std::vector<std::shared_ptr<EventReceiver>>(collectors.begin(), collectors.end()));
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