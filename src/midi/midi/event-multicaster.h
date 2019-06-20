#ifndef EVENT_MULTICASTER_H
#define EVENT_MULTICASTER_H

#include <vector>
#include "midi-format.h"
#include "eventreceiver.h"

namespace midi
{
	class EventMulticaster
	{
		std::vector<std::shared_ptr<midi::EventReceiver>> receivers;

	public:
		EventMulticaster(std::vector<std::shared_ptr<midi::EventReceiver>> receivers) : receivers(receivers)
		{
		}

		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size);
		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size);
		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity);
		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity);
		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure);
		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value);
		void program_change(Duration dt, Channel channel, Instrument program);
		void channel_pressure(Duration dt, Channel channel, uint8_t pressure);
		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value);
	};
}

#endif