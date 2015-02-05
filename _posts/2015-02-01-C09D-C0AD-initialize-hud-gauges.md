---
layout: post
title:  Initialize HUD gauges ($C09D—C0AD)
---

```
C09D: 10 8E C0 A9  LDY #$C0A9  Set Y = $C0A9
C0A1: 8E 1D 00     LDX #$1D00  Set X = $1D00
C0A4: BD D8 6C     JSR $D86C   Jump with return
C0A7: 20 05        BRA $C0AE   Jump
```

We're setting a couple of address registers before we jump to a subroutine. Without any true certainty, we can take a guess at what's going to happen based on these registers.

The X address refers to an arbitrary area in RAM that we haven't previously touched; the "nice round number" of the address suggests that we might be loading some variables or data into here. 

The Y register refers to some instructions in the cartridge ROM which start just a few bytes ahead at `$C0A9`:

```
C0A9: 46           RORA
C0AA: 54           LSRB
C0AB: 53           COMB
C0AC: 52           ?????
COAD: 04 C6        LSR $C6
```

The disassembler has interpreted these bytes as 8-bit instructions that don't make a lot of sense, and one instruction at `$C0AC` that can't be decoded at all (the 6809 has no valid operation for the instruction `52`). Whenever we see this, we can often assume that these are not instructions for the CPU to process, but simply data. Most disassemblers are not sophisticated enough to tell the difference and, to be fair, it's sometimes next to impossible to know the difference until the program is executing.

Look at what happens if we try interpreting these bytes as ASCII characters instead:

```
C0A9: 46           'F'
C0AA: 54           'T'
C0AB: 53           'S'
C0AC: 52           'R'
COAD: 04           EOT (end of transmission)
```

Now *this* looks promising, and rather familiar, as our in-game gauges use exactly these abbreviations:

![F T S R Gauges](../images/FTSR_full.png "F T S R Gauges")

The byte `04` at `$C0AD` is not a renderable ASCII character, but its definition as an "end of transmission" marker suggests that it may signal the point at which we should stop interpreting bytes as text.

With these clues it would be fair to guess that we're going to be rendering the gauges to the screen.

