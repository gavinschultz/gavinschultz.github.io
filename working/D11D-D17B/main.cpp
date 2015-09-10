#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "USR_random.h"
#include "cpp_random.h"

uint8_t memory[8192 * 4] = { 0 };
void dump_memory(uint16_t offset, uint16_t length);
void USR_random_number();
void _ld_ext(uint8_t** cpu_register, uint16_t address);
void _ld_ext(uint8_t* cpu_register, uint16_t address);
void _ld(uint8_t* cpu_register, uint8_t value);
void _ld(uint8_t* cpu_register, uint8_t* from_cpu_register);
void _st(uint8_t* cpu_register, uint8_t value);
void _st(uint8_t* cpu_register, uint16_t value);
void _st8(uint8_t* cpu_register, uint8_t* address);
void _pshs8(uint8_t cpu_register);
void _pshs8(uint8_t* cpu_register);
void _pshs16(uint8_t* cpu_register);
uint8_t* S;
uint8_t* X;
uint8_t A;
uint8_t B;

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    rand(); rand(); rand(); // rand() not very good so give it a couple runs
    _ld_ext(&S, 0x4000);

    uint16_t i;
    // D11D - D12B
    for (i = 0x2129; i < 0x2229; i += 8)
    {
        _ld_ext(&X, i);
        _st(X + 2, (uint16_t)0x0);
    }

    // D12C - D136
    _ld_ext(&X, 0x212C);

    uint32_t new_seed = 0x403f403f;
    reseed(new_seed);
    random_number();
    print_console();
    reseed(new_seed);

    /*
    D134: 8E 21 2C     LDX #$212C   X = $212C
    D137: A6 84        LDA ,X       A = X->val
    D139: A0 E4        SUBA ,S      A = A - random number byte 1
    D13B: 2A 01        BPL $D13E    Branch if the result is >= 0
    D13D: 40           NEGA         A = -A
    D13E: 81 03        CMPA #$03    A <= 3?
    D140: 23 14        BLS $D156    Jump to $D156 if A <= 3
    D142: 30 08        LEAX +$08,X  X = X + 8
    D144: 8C 22 2C     CMPX #$222C  Check if X has reached $222C
    D147: 26 EE        BNE $D137    Jump back to $D137 if not
    */
    int failcount = 0;
    i = 0x212C;
    do
    {
        /*
        D12F: BD DB 1B      JSR $DB1B    Call random_number()
        D132: 34 12        PSHS, X, A    Push X and random number byte 1 to the stack
        */
        USR_random_number();
        uint8_t proposed_byte = A;
        uint8_t* current_insert_position = X;
        for (uint16_t j = 0x212C; j < 0x222C; j += 8)
        {
            _ld_ext(&A, (uint16_t)j);
            A -= proposed_byte;
            if ((int8_t)A < 0)
                A = -A;
            if (A <= 3)
            {
                /*
                D156: BD DB 1B     JSR $DB1B
                D159: A7 E4        STA ,S
                D15B: 20 D7        BRA $D134
                */
                printf("Resetting at %04X, proposed_byte %02X does not match %02X (miss by %02X) at %04X\n", j, proposed_byte, *(j + memory), A, (X - memory));
                USR_random_number();
                proposed_byte = A;
                j = 0x212C - 8;
                failcount++;
                //dump_memory(0x2129, 0xFF);
                continue;
            }
            if (failcount > 1000)
            {
                printf("Fail count %i, quitting.", failcount);
                break;
            }
        }

        /*
        D149: 35 12        PULS ,A,X    Pop random byte 1 and X off the stack
        D14B: A7 84        STA ,X       X->val = random byte
        D14D: 30 08        LEAX +$08,X  X = X + 8
        D14F: 8C 22 2C     CMPX #$222C  Check if X has reached $222C
        D152: 26 DB        BNE $D12F    Jump back to $D12F if not
        D154: 20 07        BRA $D15D
        */
        _st(X, (uint8_t)proposed_byte);
        i += 8;
        X += 8;
    } while (i <= 0x222C);

    

    /*
    D15D: 8E 21 2B     LDX #$212B
    D160: 86 5D        LDA #$5D
    D162: 8D 0E        BSR $1172
    D164: 86 68        LDA #$68
    D166: 8D 0A        BSR $1172
    D168: 86 70        LDA #$70
    D16A: 8D 06        BSR $1172
    D16C: 86 78        LDA #$78
    D16E: 8D 02        BSR $1172
    */
    _ld_ext(&X, 0x212B);
    for (int i = 0; i < 256; i += 8)
    {
        if (i < 64)
            _st(X + i, (uint8_t)0x5D);
        else if (i < 128)
            _st(X + i, (uint8_t)0x68);
        else if (i < 196)
            _st(X + i, (uint8_t)0x70);
        else
            _st(X + i, (uint8_t)0x78);
    }

    /*
    D170: 20 0A        BRA $117C
    D172: C6 08        LDB #$08
    D174: A7 84        STA ,X
    D176: 30 08        LEAX +$08,X
    D178: 5A           DECB
    D179: 26 F9        BNE $1174
    D17B: 39           RTS
    */



    printf("A: %02X\tB: %02X\n", A, B);
    printf("S: %02X\n", S - memory);

    dump_memory(0x2129, 0xFF);
    printf("\nStack\n-----\n");
    dump_memory(0x3FF0, 0xF);
    return 0;
}

void _ld_ext(uint8_t** cpu_register, uint16_t address)
{
    *cpu_register = (uint8_t*)(memory + address);
}
void _ld_ext(uint8_t* cpu_register, uint16_t address)
{
    *cpu_register = *((uint8_t*)(memory + address));
}
void _ld(uint8_t* cpu_register, uint16_t address)
{
    *cpu_register = *((uint8_t*)(memory + address));
}
void _ld(uint8_t* cpu_register, uint8_t value)
{
    *cpu_register = value;
}
void _ld(uint8_t* cpu_register, uint8_t* from_cpu_register)
{
    *cpu_register = *from_cpu_register;
}

void _st(uint8_t* cpu_register, uint8_t value)
{
    *cpu_register = value;
}
void _st(uint8_t* cpu_register, uint16_t value)
{
    uint8_t* address = cpu_register;
    *address = (value >> 8);
    address++;
    *address = value & 0xFF;
}
void _st8(uint8_t* cpu_register, uint8_t* address)
{
    *address = *cpu_register;
}

void _pshs8(uint8_t* cpu_register)
{
    S -= 1;
    _st(S, *cpu_register);
}
void _pshs8(uint8_t cpu_register)
{
    S -= 1;
    _st(S, cpu_register);
}
void _pshs16(uint8_t* cpu_register)
{
    S -= 2;
    _st(S, (uint8_t)((cpu_register - memory) >> 8));
    _st(S + 1, (uint8_t)((cpu_register - memory) & 0xFF));
}

void USR_random_number()
{
    //_ld(&A, random_number());
    //_ld(&A, random_number_cpp());
    //_ld(&A, (uint8_t)(rand() & 0xFF));
    _ld(&A, (uint8_t)((rand() >> 8) & 0xFF));

    //uint32_t val = rand();
    //_ld(&A, (uint8_t)(val >> 8));
    //_ld(&B, (uint8_t)(val & 0xFF));    
}

void dump_memory(uint16_t offset, uint16_t length)
{
    for (int i = offset; i < offset + length; i += 8)
    {
        printf("%04X: %02X%02X %02X%02X %02X%02X %02X%02X\n",
            i,
            memory[i + 0], memory[i + 1], memory[i + 2], memory[i + 3],
            memory[i + 4], memory[i + 5], memory[i + 6], memory[i + 7]);
    }
}