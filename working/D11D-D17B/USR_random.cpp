#include "USR_random.h"

uint8_t register_A = 0;
uint8_t carry_bit = 0;
uint8_t byte_A = 0xFF; // $26FA
uint8_t byte_B = 0xFF; // $26FB
uint8_t byte_C = 0xFF; // $26FC
uint8_t byte_D = 0xFF; // $26FD

void reseed(uint32_t seed)
{
    byte_A = seed >> 24;
    byte_B = (seed >> 16) & 0xFF;
    byte_C = (seed >> 8) & 0xFF;
    byte_D = seed & 0xFF;
}

int print_console()
{
    for (int i = 1; i < 100; i++)
    {
        printf("%06i: %02X %02X %02X %02X\n", i, byte_A, byte_B, byte_C, byte_D);
        random_number();
    }
    return 1;
}

// Rotates a given byte one bit to the left, taking the carry_bit as the LSB and
// storing the old MSB back to the carry_bit
void asm_ROL(uint8_t* i)
{
    uint8_t new_carry_bit = (*i & 0x80) >> 7;
    *i = (*i << 1) + carry_bit;
    carry_bit = new_carry_bit;
}

// Shifts a given byte one bit to the left. Like asm_ROL, this sets the carry-bit, 
// but does not use it (the LSB is always set to 0).
void asm_ASL(uint8_t* i)
{
    carry_bit = (*i & 0x80) >> 7;
    *i <<= 1;
}

uint8_t random_number()
{
    register_A = byte_A;

    asm_ASL(&register_A);
    asm_ASL(&register_A);
    asm_ASL(&register_A);
    register_A ^= byte_A;
    asm_ASL(&register_A);

    asm_ROL(&byte_D);
    asm_ROL(&byte_C);
    asm_ROL(&byte_B);
    asm_ROL(&byte_A);

    if (byte_A == 0 && byte_B == 0 && byte_C == 0 && byte_D == 0)
    {
        reseed(0xFFFFFFFF);
    }

    return byte_D;
}