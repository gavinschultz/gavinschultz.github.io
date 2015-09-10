#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <string>

uint8_t func_DB1B();
void reset_bytes();
void asm_ROL(uint8_t*);
void asm_ASL(uint8_t*);
int print_console();
int write_bmp(char* path, int mod);

uint8_t register_A = 0;
uint8_t carry_bit = 0;
uint8_t byte_A = 0; // $26FA
uint8_t byte_B = 0; // $26FB
uint8_t byte_C = 0; // $26FC
uint8_t byte_D = 0; // $26FD

int main(int argc, char* argv[])
{
    reset_bytes();

    if (argc == 2 && strcmp(argv[1], "print-console") == 0)
    {
        return print_console();
    }

    if (argc == 3 && (strcmp(argv[1], "bitmap1") == 0 || strcmp(argv[1], "bitmap8") == 0))
    {
        return write_bmp(argv[2], strcmp(argv[1], "bitmap1") == 0 ? 1 : 8);
    }

    std::cout << "\nUsage:\n"
        << "  db1b print-console       Performs 10,000 iterations and write the resulting four bytes to the console.\n"
        << "  db1b bitmap1 \"{path}\"    Performs 256K iterations and generates a 1MB bitmap of the fourth byte.\n"
        << "  db1b bitmap8 \"{path}\"    Performs 2M iterations with 8 iterations between each read and generates a 1MB bitmap of the fourth byte.\n";
    return 0;
}

void reset_bytes()
{
    // These are the initial values set for these bytes in $C016-C01E, and also 
    // what the values are reset to within the function itself if all 4 bytes 
    // end up at zero.
    byte_A = 0xFF;
    byte_B = 0xFF;
    byte_C = 0xFF;
    byte_D = 0xFF;
}

int print_console()
{
    for (int i = 1; i < 10000; i++)
    {
        printf("%06i: %02X %02X %02X %02X\n", i, byte_A, byte_B, byte_C, byte_D);
        func_DB1B();
    }
    return 1;
}

int write_bmp(char* path, int mod)
{
    std::FILE* file = std::fopen(path, "wb");
    if (file == NULL)
    {
        printf("The file '%s' could not be opened for writing.", path);
        return 0;
    }

    uint8_t bmp_header[] = {
        'B', 'M',                   // 00h: ID field = windows BMP
        0x36, 0x00, 0x10, 0,        // 02h: total file size = 1MB + 54 bytes headers
        0, 0,                       // 06h: app-specific 1
        0, 0,                       // 08h: app-specific 2
        0x36, 0, 0, 0               // 0Ah: offset for pixel data = 36h
    };
    uint8_t dib_header[] = {
        40, 0, 0, 0,                // 0Eh: 40 bytes in DIB header
        0, 0x02, 0, 0,              // 12h: 512 pixels in width
        0, 0x02, 0, 0,              // 16h: 512 pixels in height
        1, 0,                       // 1Ah: 1 color planes
        32, 0,                      // 1Ch: 32 bits per pixel
        0, 0, 0, 0,                 // 1Eh: compression = none
        0x00, 0x00, 0x04, 0,        // 22h: size of raw bitmap data (262144 bytes)
        0xC4, 0x0E, 0, 0,           // 26h: dpi horizontal = 3780 pixels/m (96 dpi)
        0xC4, 0x0E, 0, 0,           // 2Ah: dpi vertical= 3780 pixels/m (96 dpi)
        0, 0, 0, 0,                 // 2Eh: 0 colors in palette 
        0, 0, 0, 0                  // 32h: 0 important colors
    };
    std::fwrite(bmp_header, sizeof(uint8_t), sizeof(bmp_header), file);
    std::fwrite(dib_header, sizeof(uint8_t), sizeof(dib_header), file);
    for (int i = 0; i < 512 * 512 * mod; i++)
    {
        if (i % mod == 0)
        {
            uint8_t buffer[] = { byte_D, byte_D, byte_D, 0 };
            std::fwrite(buffer, sizeof(uint8_t), sizeof(buffer), file);
        }
        func_DB1B();
    }
    std::fclose(file);

    return 1;
}

uint8_t func_DB1B()
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
        reset_bytes();
    }

    return byte_D;
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