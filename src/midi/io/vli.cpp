#include "vli.h"
#include "read.h"

uint64_t io::read_variable_length_integer(std::istream& in)
{
	uint64_t result = 0;

	uint8_t c;
	do {
		c = read<uint8_t>(in);
		result = (result << 7) | (uint64_t)(c & 127);
	} while (c > 127);

	return result;
}
