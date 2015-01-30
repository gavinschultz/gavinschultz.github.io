---
layout: post
title:  Set skill-based variable 2 ($006F—0081)
---

```
006F: B6 27 00     LDA $2700    Set A = skill_level
0072: 4A           DECA         skill_level - 1
0073: 26 05        BNE $007A    Branch to $007A if skill_level - 1 > 0
0075: CC 00 80     LDD #$0080   Set D = 128
0078: 20 06        BRA $0080    Branch to $0080
007A: C6 10        LDB #$10     Set B = 16
007C: 3D           MUL          (skill_level - 1) x 16
007D: C3 00 80     ADDD #$0080  + 128
0080: DD 65        STD $65      Store result to $2065
```

This is doing a very similar thing to the previous calculation using the skill level, but with a multiplier of 16 instead of 80.

<div>$$
\begin{array}{l@{\,}l}
    y & = ((\mathtt{skill\_level} - 1)\times16) + 128
\end{array}
$$</div>

Oddly, this time there's some branching which will avoid extra calculations when the skill_level is 1. The resulting zero in the formula means we can set the result directly to 128. This strikes me as an unnecessary optimisation to be honest; for those 7 extra bytes of program space, we save a few CPU cycles in a non-critical function, but only when the user is playing on the easiest level. It would not make any noticeable difference to the player.

Again we don't know exactly what this variable is used for, but the possible values are:

| skill | value       |
|:-----:|-------------|
|   1   | 128 (`$80`) |
|   2   | 144 (`$90`) |
|   3   | 160 (`$A0`) |
|   4   | 176 (`$B0`) |
|   5   | 192 (`$C0`) |
|   6   | 208 (`$D0`) |
|   7   | 224 (`$E0`) |
|   8   | 240 (`$F0`) |

This time the result is never more than 8 bits, so there's no extra instructions needed to deal with 16-bit results.