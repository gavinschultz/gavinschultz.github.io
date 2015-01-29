---
layout: post
title:  Unknown 16-bit multiplication, sort of ($004B—006E)
---

```
004B: 0F 39        CLR $39		Set 0x2039 to 0
```

This perhaps signifies a single byte variable, but it doesn't seem to be used anytime soon. We'll have to come back to it later.

```
004D: B6 27 00     LDA $2700    Set A = 8 (0x2700 previously set to 8)
0050: 4A           DECA         8 -> 7
0051: C6 50        LDB #$50     Set B = 80
0053: 3D           MUL          80 x 7 = 560
0054: C3 00 80     ADDD #$0080  560 + 128 = 688 (02B0)
0057: DD 45        STD $45      Store 0x2045 = 02B0
0059: DD 47        STD $47      Store 0x2047 = 02B0
```

Back at 0x0010, we set 0x2700 to 8. We use that value now to perform a calculation:

<div>$$
\begin{array}{l@{\,}l}
    x & = ((\mathtt{(value\ at\ 0x2700)} - 1)\times80) + 128 \\
    & = ((8 - 1)\times80) + 128 \\
     & = 688
\end{array}
$$</div>

No idea on what this number signifies yet.

```
005B: 96 46        LDA $46      Set A = 176 (0xB0)
005D: C6 55        LDB #$55     Set B = 85
005F: 3D           MUL          176 x 85 = 14960 (0x3A70)
```

I'm glad this processor has its own multiply instruction. It takes 11 CPU cycles, but it could be worse; apparently on the Intel 8086/8088 these instructions took up to 200 cycles.

Anyhow, we've multiplied only the *lower* byte of the 2-byte value we calculated and stored at 0x2045. Why? 

Performing arithmetic on just one part of a multi-byte value might look bizarre at first, but we'll probably end up multiplying the other byte later and then adding the two together. On an 8-bit processor, this is the only way to multiply two numbers that might give a result of 16-bits (or more).

In short, we're probably just trying to do this:

<div>$$
\begin{array}{l@{\,}l}
    y & = 688\times85 \\
    & = 58480
\end{array}
$$</div>

```
0060: 34 06        PSHS ,B,A    Push 3A70 to the stack
```

Now we push the result of that lower-byte multiplication to the stack. [We saw earlier]({% post_url 2015-01-25-0021-0024-initialize_stack %}) that the stack could be used to temporarily store data, and this is the first time that we're doing so, in this case on the number `3A70`.

Assuming we're still at the top of the stack at 0x3FFF, this 2-byte number will be saved at location 0x3FFD.

```
0062: 6F E2        CLR ,-S      Store 0x3FFC = 0, Set S = 3FFC
0064: 96 45        LDA $45      Set A = 2
0066: C6 55        LDB #$55     Set B = 85
0068: 3D           MUL          2 x 85 = 170 (0xAA)
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
0069: E3 E1        ADDD ,S++    Set D = 3A + AA = 00E4 (228), S = 3FFE 
006B: 32 61        LEAS +$01,S  Set S = 3FFF (top of stack)
006D: DD 49        STD $49      Store 0x2049 = 00E4
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

Also, although in this particular case, ignoring the lower byte doesn't seem all that useful, but what if we were multiplying larger numbers? For example:

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