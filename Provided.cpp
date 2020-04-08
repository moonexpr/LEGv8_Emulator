#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>

#define H_Provided

namespace Provided
{
	char printable_char(uint8_t c)
	{
		return isprint(c) ? c : '.';
	}

	void hexdump(FILE *f, int8_t *start, size_t size)
	{
		size_t i;

		for (i = 0; i < size - (size % 16); i += 16) {


		fprintf(stdout,
			"%08x "
			" %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
			" %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
			" |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
			(int32_t) i,
			start[i +  0], start[i +  1], start[i +  2], start[i +  3],
			start[i +  4], start[i +  5], start[i +  6], start[i +  7],
			start[i +  8], start[i +  9], start[i + 10], start[i + 11],
			start[i + 12], start[i + 13], start[i + 14], start[i + 15],
			printable_char(start[i +  0]), printable_char(start[i +  1]),
			printable_char(start[i +  2]), printable_char(start[i +  3]),
			printable_char(start[i +  4]), printable_char(start[i +  5]),
			printable_char(start[i +  6]), printable_char(start[i +  7]),
			printable_char(start[i +  8]), printable_char(start[i +  9]),
			printable_char(start[i + 10]), printable_char(start[i + 11]),
			printable_char(start[i + 12]), printable_char(start[i + 13]),
			printable_char(start[i + 14]), printable_char(start[i + 15]));
		}
		fprintf(stdout, "%08x\n", (int32_t) size);
	}

	int64_t sign_extend(int64_t value, int bits)
	{
		int i;
		if (value & (1 << (bits - 1))) {
			for (i = 63; i >= bits; i--) {
				value |= 1ULL << i;
			}
		}

		return value;
	}
}
