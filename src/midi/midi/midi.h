#ifndef MIDI_H
#define MIDI_H

#include "midi-format.h"
#include "eventreceiver.h"

#include "channel-note-collector.h"
#include "event-multicaster.h"
#include "note-collector.h"

namespace midi
{
	void read_chunk_header(std::istream&, CHUNK_HEADER*);
	std::string header_id(const CHUNK_HEADER&);
	void read_mthd(std::istream&, MTHD*);
	void read_mtrk(std::istream&, EventReceiver&);
	std::vector<midi::NOTE> read_notes(std::istream&);

	bool is_sysex_event(uint8_t);
	bool is_meta_event(uint8_t);
	bool is_midi_event(uint8_t);
	bool is_running_status(uint8_t);

	uint8_t extract_midi_event_type(uint8_t);
	Channel extract_midi_event_channel(uint8_t);
	bool is_note_off(uint8_t);
	bool is_note_on(uint8_t);
	bool is_polyphonic_key_pressure(uint8_t);
	bool is_control_change(uint8_t);
	bool is_program_change(uint8_t);
	bool is_channel_pressure(uint8_t);
	bool is_pitch_wheel_change(uint8_t);
}

#endif