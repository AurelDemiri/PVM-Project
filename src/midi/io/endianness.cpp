#include "endianness.h"

void io::switch_endianness(uint16_t * n)
{
	*n = (*n << 8) | (*n >> 8);
}

void io::switch_endianness(uint32_t * n)
{
	*n = ((*n << 8) & 0xFF00FF00) | ((*n >> 8) & 0xFF00FF);
	*n = (*n << 16) | (*n >> 16);
}

void io::switch_endianness(uint64_t * n)
{
	*n = ((*n << 8) & 0xFF00FF00FF00FF00ULL) | ((*n >> 8) & 0x00FF00FF00FF00FFULL);
	*n = ((*n << 16) & 0xFFFF0000FFFF0000ULL) | ((*n >> 16) & 0x0000FFFF0000FFFFULL);
	*n = (*n << 32) | (*n >> 32);
}