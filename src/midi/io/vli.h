#ifndef VLI_H
#define VLI_H

#include <sstream>

namespace io
{
	uint64_t read_variable_length_integer(std::istream&);
}

#endif