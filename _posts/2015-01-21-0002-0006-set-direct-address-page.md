---
layout: post
title: Set direct address page ($0002—0006)
---

## Set direct address page ($0002---0006)
```
0002: 86 20        LDA  #$20
0004: 1F 8B        TFR  A,DP   Set to page 32 (offset 0x2000)
```

This is essentially a technique for saving bytes in the program code. Normally, a memory address is specified by the full 16-bit address e.g.

`JSR $2010`

The direct page (DP) register specifies which memory page to use when using direct addressing mode in all subsequent instructions. In effect this sets a default value for the upper 8 bits of direct-addressed addresses:

i.e. ADDRESS = DP REGISTER + OPERAND

This means that your addresses can be specified with one byte instead of two e.g.

`JSR $10`

By implication, the 6809 considers a single memory page to be 256 bytes. There are 256 such pages available for addressing. (e.g. 256 pages x 256 bytes = 64KB).

The initial LDA is required because the DP register cannot be set directly, but can be transferred to.

**I'm not yet sure why address $2000 (8192) is chosen for the direct page. This makes most sense as an offset from the starting address of the ROM itself, which is exactly 8192 bytes (8K). But if we're talking about RAM, the CoCo makes available the data from 0600 to the top of RAM (3FFF for 16K, 7FFF for 32K). This also overlaps with the graphics pages; I don't understand how these work yet.**