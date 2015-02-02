---
layout: post
title:  Set skill-based variable 1 ($004B—006E)
---

```
C04B: 0F 39        CLR $39		Set 0x2039 to 0
```

This perhaps signifies a single byte variable, but it doesn't seem to be used anytime soon. We'll have to come back to it later.

```
C04D: B6 27 00     LDA $2700    Set A = skill_level
C032: 4A           DECA         skill_level - 1
C033: C6 50        LDB #$50     Set B = 80
C035: 3D           MUL          (skill_level - 1) x 80
C036: C3 00 80     ADDD #$0080  + 128
C039: DD 45        STD $45      Store result to 0x2045
C03B: DD 47        STD $47      Store result to 0x2047
```

Earlier we identified `$2700` as the [skill_level variable]({% post_url 2015-01-22-000E-0010-initialize-skill-level %}), which has a range of 1 to 8. We use that value now to perform a calculation:

<div>$$
\begin{array}{l@{\,}l}
    x & = ((\mathtt{skill\_level} - 1)\times80) + 128 \\
\end{array}
$$</div>

We don't know what this number signifies yet, but for skill ranges 1 to 8 it would be set to:

 - skill level 1 = 128 (`$80`)
 - skill level 2 = 208 (`$D0`)
 - skill level 3 = 288 (`$120`)
 - skill level 4 = 368 (`$170`)
 - skill level 5 = 448 (`$1C0`)
 - skill level 6 = 528 (`$210`)
 - skill level 7 = 608 (`$260`)
 - skill level 8 = 688 (`$2B0`)

For the following code, let's assume that the skill level is set to 8, so the result is `$02B0`.

```
C05B: 96 46        LDA $46      Set A = 176 ($B0)
C05D: C6 55        LDB #$55     Set B = 85
C05F: 3D           MUL          176 x 85 = 14960 ($3A70)
```

I'm glad this processor has its own multiply instruction. It takes 11 CPU cycles, but it could be worse; apparently on the Intel 8086/8088 these instructions took up to 200 cycles.

Anyhow, we've multiplied only the *lower* byte of the 2-byte value we calculated and stored at `$2045`. Why? 

Performing arithmetic on just one part of a multi-byte value might look strange at first, but we'll probably end up multiplying the other byte later and then adding the two together. On an 8-bit processor, this is the only way to multiply two numbers that might give a result of 16 (or more) bits.

In short, we're probably just trying to do this:

<div>$$
\begin{array}{l@{\,}l}
    y & = 688\times85 = 58480
\end{array}
$$</div>

```
C03C: 34 06        PSHS ,B,A    Push 3A70 to the stack
```

Now we push the result of that lower-byte multiplication to the stack. [We saw earlier]({% post_url 2015-01-25-0021-0024-initialize_stack %}) that the stack could be used to temporarily store data; this is the first time we're actually doing so, in this case on the number `$3A70`.

Assuming we're still at the top of the stack at 0x3FFF, this 2-byte number will be saved at location 0x3FFD.

```
C03E: 6F E2        CLR ,-S      Store 0x3FFC = 0, Set S = 3FFC
C040: 96 45        LDA $45      Set A = 2
C042: C6 55        LDB #$55     Set B = 85
C044: 3D           MUL          2 x 85 = 170 (0xAA)
```

Sure enough, we've now performed the multiplication on just the *upper* byte of the value we generated. I would now expect that we add these two together to get E470.

<div>$$
\begin{array}{r@{\,}r}
      & \mathtt{3A70} \\
    + & \mathtt{AA00} \\
    \hline
      & \mathtt{E470} \\
\end{array}
$$</div>

But in 0x0062, why did we decrement the stack one extra byte, and clear that value?

```
C045: E3 E1        ADDD ,S++    Set D = 3A + AA = 00E4 (228), S = 3FFE 
C06B: 32 61        LEAS +$01,S  Set S = 3FFF (top of stack)
C06D: DD 49        STD $49      Store 0x2049 = 00E4
```

We haven't quite done what I expected. We are doing an addition, but by

 1. pushing the stack down an extra byte and
 2. leaving the D register containing only the upper-byte multiplication (i.e. `00AA`)

we actually end up ignoring the lower byte entirely.

<div>$$
\begin{array}{r@{\,}r}
      & \mathtt{003A} \\
    + & \mathtt{00AA} \\
    \hline
      & \mathtt{00E4} \\
\end{array}
$$</div>

It's hard to say why we do that as we don't yet know what this variable is used for. But if we just ignore the lower byte and assume it's zero, the value may be accurate enough for us i.e.

<div>$$
\mathtt{E400}\approx\mathtt{E470} \\
58368\approx58480
$$</div>

With a skill level of between 1 and 8, all possible values are shown below:

| skill |           value | value + lower-byte | value at `$2049` |
|:-----:|----------------|-------------------|-----------------|
|   1   | 10752 (`$2A00`) |    10880 (`$2A80`) |     42 (`$002A`) |
|   2   | 17664 (`$4500`) |    17680 (`$4510`) |     69 (`$0045`) |
|   3   | 24320 (`$5F00`) |    24480 (`$5FA0`) |     95 (`$005F`) |
|   4   | 31232 (`$7A00`) |    31280 (`$7A30`) |    122 (`$007A`) |
|   5   | 37888 (`$9400`) |    38080 (`$94C0`) |    148 (`$0094`) |
|   6   | 44800 (`$AF00`) |    44880 (`$AF50`) |    175 (`$00AF`) |
|   7   | 51456 (`$C900`) |    51680 (`$C9E0`) |    201 (`$00C9`) |
|   8   | 58368 (`$E400`) |    58480 (`$E470`) |    228 (`$00E4`) |

### Discarding the lower byte seems pointless...
In this particular case ignoring the lower byte doesn't seem all that useful, but it certainly would be if we were multiplying larger numbers. For example:

<div>$$
\begin{array}{r@{\,}r}
      & \mathtt{FFB0} \\
    \times & \mathtt{0055} \\
    \hline
      & \mathtt{54E570} \\
\end{array}
$$</div>

Now we have a 24-bit number that we have to manage. This is much more awkward than a 16-bit number, because we don't even have any registers on the CPU capable of storing the whole number at once. And again, if we just pretend that the lower byte doesn't matter and assume it's zero, the end result is not that much different and much easier to manage. e.g. 

<div>$$
\mathtt{54E500}\approx\mathtt{54E570} \\
5,563,648\approx5,563,760
$$</div>

### ...but this doesn't make sense
Two things still don't make sense to me:

1. All possible values are still 16-bit; we would only start to exceed that if the skill_level went to 10 or higher. As-is, the value at `$2049` will *always* be `$00`.
2. I can't figure out is why we did a `PSHS ,B,A` and not just a `PSHS ,A`. The B register, containing the ignored lower byte in 3FFE, was never touched, so why put it in the stack at all? Not pushing the B register would also have avoided the issue an extra instruction to return to the top of the stack, i.e. the instruction `LEAS +$01,S` would not have been required.
