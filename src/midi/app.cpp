#ifndef TEST_BUILD

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include "shell/command-line-parser.h"
#include "imaging/bitmap.h"
#include "imaging/bmp-format.h"
#include "midi/midi.h"

void draw_rectangle(imaging::Bitmap& bitmap, const Position& pos, const uint32_t& width, const uint32_t& height, const imaging::Color& color)
{
	for (uint32_t i = 0; i < width; i++)
	{
		for (uint32_t j = 0; j < height; j++)
		{
			bitmap[Position(pos.x + i, pos.y + j)] = color;
		}
	}
}

std::unique_ptr<imaging::Bitmap> create_full_frame(const std::vector<midi::NOTE>& notes, const uint32_t& scale, const uint32_t& height_of_note)
{
	// My first version had a slice here, now it doesn't and it's spaghetti.

	const auto last_note = std::max_element(notes.begin(), notes.end(), [](const midi::NOTE &a, const midi::NOTE &b) { return (a.start + a.duration) < (b.start + b.duration); })[0];
	const uint32_t bitmap_width = static_cast<uint32_t>(scale * (value(last_note.start + last_note.duration) / 1000.0f));

	const uint32_t max_bitmap_height = 128 * height_of_note;
	const auto minmax_note_number = std::minmax_element(notes.begin(), notes.end(), [](const midi::NOTE &a, const midi::NOTE &b) { return a.note_number < b.note_number; });
	const uint32_t min_y = max_bitmap_height - value((*minmax_note_number.second).note_number) * height_of_note;
	const uint32_t max_y = max_bitmap_height - value((*minmax_note_number.first).note_number) * height_of_note;
	const uint32_t bitmap_height = max_y - min_y + height_of_note;

	// Changed Bitmap constructor for performance, compiler optimization probably does the same thing.
	auto bitmap = std::make_unique<imaging::Bitmap>(bitmap_width, bitmap_height);

	for (auto& note : notes)
	{
		uint32_t x = static_cast<uint32_t>(scale * (value(note.start) / 1000.0f));
		uint32_t y = max_bitmap_height - min_y - (value(note.note_number) * height_of_note);
		uint32_t width = static_cast<uint32_t>(scale * (value(note.duration) / 1000.0f));

		// TODO: Make pretty colors
		imaging::Color color = imaging::Color { 
			value(note.instrument) > 42 ? 0.0f : 1.0f, 
			value(note.instrument) % 2 ? 1.0 - static_cast<double>(value(note.instrument)) / 128L : 0,
			static_cast<double>(value(note.instrument)) / 128L 
		};
		draw_rectangle(*bitmap.get(), Position(x, y), width, height_of_note, color);
	}

	return std::move(bitmap);
}

void save_frame(const imaging::Bitmap& bitmap, std::string filename, const uint32_t& n)
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(5) << n;

	size_t x = filename.find("%d");
	if (x != std::string::npos)
		filename.replace(x, 2, ss.str());

	imaging::save_as_bmp(filename, bitmap);
}

int main(int argc, char** argv)
{
	// Stopwatch start
	auto start_time = std::chrono::high_resolution_clock::now();

	uint32_t frame_width = 0;
	uint32_t step = 1;
	uint32_t scale = 50;
	uint32_t height_of_note = 16;

	shell::CommandLineParser parser;
	parser.add_argument(std::string("-w"), &frame_width);
	parser.add_argument(std::string("-d"), &step);
	parser.add_argument(std::string("-s"), &scale);
	parser.add_argument(std::string("-h"), &height_of_note);
	parser.process(argc, argv);

	if (parser.positional_arguments().size() != 2)
	{
		std::cout << "Please provide an input and output file" << std::endl;
		return 0;
	}

	std::ifstream infile(parser.positional_arguments()[0], std::ios_base::binary);
	if (infile.is_open())
	{
		std::vector<midi::NOTE> notes = midi::read_notes(infile);
		infile.close();

		const auto full_frame = create_full_frame(notes, scale, height_of_note);

		if (frame_width == 0 || frame_width > (*full_frame.get()).width())
		{
			save_frame(*full_frame.get(), parser.positional_arguments()[1], 0);
		}
		else
		{
			for (uint32_t i = 0; i < (*full_frame.get()).width() - frame_width; i += step)
			{
				const auto pizza = (*full_frame.get()).slice(i, 0, frame_width, (*full_frame.get()).height());
				save_frame(*pizza.get(), parser.positional_arguments()[1], i);
			}
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		std::cout << "Completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds!" << std::endl;
	}
	else
	{
		std::cout << "Could not open input file!" << std::endl;
	}

	return 0;
}

#endif
