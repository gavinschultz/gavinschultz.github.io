#include <stdio.h>
#include <stdint.h>

uint16_t f1(uint8_t i)
{
	uint16_t j = (i * 80) + 128;
	return j;
}

int main()
{
	 uint8_t var2700 = 8;
	 uint16_t var2045;
	 uint16_t var2047;
	 uint32_t var2049;

	var2045 = f1(var2700 - 1);
	var2047 = f1(var2700 - 1);
	var2049 = var2045 * 85;

	printf("2045: %04X\n", var2045);
	printf("2047: %04X\n", var2047);
	printf("2049: %08X\n", var2049);

	return 0;
}