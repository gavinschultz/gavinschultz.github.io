#include <stdio.h>
#include <stdint.h>
#include "fileloader.h"
#include <array>

std::array<uint8_t, 0xFFFF+1> data;

void func1(uint16_t src_address, uint16_t destination_address)
{
	const uint16_t ALPHA_GLYPH_ADDRESS_START = 0xD977;
	const uint16_t QUESTION_GLYPH_ADDRESS_START = 0xD8C7;

	for (;;)
	{
		// D86E
		uint16_t ascii_character = data[src_address++];

		printf("%04X: %04X\n", src_address, ascii_character);

		// D870-D873
		if (ascii_character == 4) // EOD
		{
			printf("Termination code %02X", ascii_character);
			return;
		}

		// D874-D88F - special case for spaces
		if (ascii_character == ' ')
		{
			for (int i = 8; i > 0; i--)
			{
				data[destination_address] = 0;
				printf("Writing %04X: %04X\n", destination_address, data[destination_address]);
				destination_address += 0x0020;
			}
		}

		uint16_t glyph_address = 0;

		// D890-D89F - special case for question marks
		if (ascii_character == '?')
			glyph_address = QUESTION_GLYPH_ADDRESS_START;
		else
		{
			// D8A0-????
			glyph_address = ALPHA_GLYPH_ADDRESS_START + ((ascii_character - 'A') * 16);
		}

		for (int i = 8; i > 0; i--)
		{
			data[destination_address] = data[glyph_address];
			printf("Writing %04X: %04X from %04X\n", destination_address, data[destination_address], glyph_address);
			glyph_address += 0x02;
			destination_address += 0x0020;
		}
	}
}

int main(int argc, char* argv[])
{
	std::printf("Loading memory from %s... ", argv[1]);
	FileLoader file_loader{ argv[1], "rb" }; // Make sure "b" for binary mode
	FILE* fp = file_loader();

	int c;
	uint16_t address = 0x0000;
	while ((c = std::fgetc(fp)) != EOF)
	{
		//if (address % 16 == 0)
		//{
		//	std::printf("%04X: ", address);
		//}
		//std::printf("%02X ", c);
		//if (address % 16 == 15)
		//{
		//	std::printf("\n");
		//}

		if (address >= data.size())
		{
			std::puts("error, input file size exceeds maximum size 0xFFFF");
			break;
		}

		data[address++] = c;
	}

	if (std::ferror(fp))
		std::puts("I/O error when reading");
	if (std::feof(fp))
		std::puts("loaded.");

	func1(0xC0A9, 0x1D00);

	return 0;
}