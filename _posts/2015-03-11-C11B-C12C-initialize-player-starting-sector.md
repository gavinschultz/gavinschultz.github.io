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

We're again generating a random number in the range 0---63, which suggests another placement into a sector of the galaxy map. Only a single position is generated, and the only thing that we *haven't* placed on the map is the player themselves; this is the initial sector into which they are spawned.

```
C124: 1F 89        TFR A,B    Copy A -> B
C126: 58           ASLB       B * 2
C127: 58           ASLB       B * 2
C128: 8E 25 19     LDX #$2519 Set X = $2519
C12B: 3A           ABX        X = X + B
C12C: 9F 36        STX $36    Store X in $2036
```

Using the same sector number, we generate an offset into galaxy map memory, almost exactly as we did for the alien population in `$C0B2-C0C0`. But this time we don't write to that memory location; we instead store the address itself at location `$2036`.

For example, suppose the player is randomly chosen to start in sector 10 (`$0A`). From this we create an address offset:

$$
\begin{array}{l@{\,}l}
X &= \mathtt{$2519} + (10\times2\times2) \\
&= \mathtt{$2541}
\end{array}
$$

```
C12E: BD D3 79     JSR $D379   Jump
```

```
D379: D6 2A        LDB $2A
D37B: D7 29        STB $29
D37D: 96 29        LDA $29
D37F: 4C           INCA
D380: FD 26 FA     STD $26FA
D383: FD 26 FC     STD $26FC
D386: 9E 36        LDX $36
D388: 27 06        BEQ $1390
D38A: 96 35        LDA $35
D38C: AB 84        ADDA ,X
D38E: A7 84        STA ,X
D390: 96 6F        LDA $6F
D392: A7 01        STA +$01,X
D394: D6 29        LDB $29
D396: 58           ASLB
D397: 58           ASLB
D398: 8E 25 19     LDX #$2519
D39B: 3A           ABX
D39C: 9F 36        STX $36
D39E: BD D1 1D     JSR $D11D
D3A1: 9E 36        LDX $36
D3A3: A6 84        LDA ,X
D3A5: B1 27 00     CMPA $2700
D3A8: 23 0A        BLS $13B4
D3AA: B0 27 00     SUBA $2700
D3AD: A7 84        STA ,X
D3AF: B6 27 00     LDA $2700
D3B2: 20 02        BRA $13B6
D3B4: 6F 84        CLR ,X
D3B6: 97 35        STA $35
D3B8: 1F 89        TFR A,B
D3BA: BD D1 CF     JSR $D1CF
D3BD: 9E 36        LDX $36
D3BF: A6 01        LDA +$01,X
D3C1: 26 07        BNE $13CA
D3C3: 0F 6B        CLR $6B
D3C5: 0F 6C        CLR $6C
D3C7: 0F 6F        CLR $6F
D3C9: 39           RTS
```