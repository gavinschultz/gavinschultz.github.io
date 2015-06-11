---
layout: post
title:  Initialize player starting sector ($C11B—C12C)
---

```
C11B: BD DB 1B     JSR $DB1B  Call random_number()
C11E: 81 40        CMPA #$40  A - 64
C120: 24 F9        BCC $011B  Call random_number() while A > 63
C122: 97 2A        STA $2A    Store A in $202A
```

We're again generating a random number in the range 0---63, which suggests another placement into a sector of the galaxy map. Only a single position is generated, and the only thing that we *haven't* placed on the map is the player themselves; this is the initial sector into which they are spawned. We will in future refer to the data at $202A as the variable `player_sector`.

```
C124: 1F 89        TFR A,B    Copy A -> B
C126: 58           ASLB       B * 2
C127: 58           ASLB       B * 2
C128: 8E 25 19     LDX #$2519 Set X = $2519
C12B: 3A           ABX        X = X + B
C12C: 9F 36        STX $36    Store X in $2036
```

Using the same sector number, we generate an offset into galaxy map memory, almost exactly as we did for the alien population in `$C0B2-C0C0`. 

For example, suppose the player is randomly chosen to start in sector 10 (`$0A`). From this we create an address offset:

$$
\begin{array}{l@{\,}l}
X &= \mathtt{$2519} + (10\times2\times2) \\
&= \mathtt{$2541}
\end{array}
$$

But this time we don't write to that memory location; we instead store the address itself (e.g. `$2541`) at address `$2036`. In other words, we *will* be tracking the player's position in the galaxy, but it won't be recorded in the galaxy map itself.

