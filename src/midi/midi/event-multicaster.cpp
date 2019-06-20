#include "event-multicaster.h"

void midi::EventMulticaster::meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	for (auto& n : receivers)
		n->meta(dt, type, std::move(data), data_size);
}

void midi::EventMulticaster::sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	for (auto& n : receivers)
		n->sysex(dt, std::move(data), data_size);
}

void midi::EventMulticaster::note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	for (auto& n : receivers)
		n->note_on(dt, channel, note, velocity);
}

void midi::EventMulticaster::note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	for (auto& n : receivers)
		n->note_off(dt, channel, note, velocity);
}

void midi::EventMulticaster::polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure)
{
	for (auto& n : receivers)
		n->polyphonic_key_pressure(dt, channel, note, pressure);
}

void midi::EventMulticaster::control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value)
{
	for (auto& n : receivers)
		n->control_change(dt, channel, controller, value);
}

void midi::EventMulticaster::program_change(Duration dt, Channel channel, Instrument program)
{
	for (auto& n : receivers)
		n->program_change(dt, channel, program);
}

void midi::EventMulticaster::channel_pressure(Duration dt, Channel channel, uint8_t pressure)
{
	for (auto& n : receivers)
		n->channel_pressure(dt, channel, pressure);
}

void midi::EventMulticaster::pitch_wheel_change(Duration dt, Channel channel, uint16_t value)
{
	for (auto& n : receivers)
		n->pitch_wheel_change(dt, channel, value);
}