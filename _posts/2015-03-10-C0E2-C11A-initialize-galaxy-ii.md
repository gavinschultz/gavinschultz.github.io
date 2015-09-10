---
layout: post
title:  Initialize galaxy II ($C0E2—C11A)
---

```
C0E2: 86 04        LDA #$04   Set A = 4
C0E4: C6 01        LDB #$01   Set B = 1
C0E6: 8D 08        BSR $C0F0  Branch
C0E8: 86 0C        LDA #$0C   Set A = 12
C0EA: C6 00        LDB #$00   Set B = 0
C0EC: 8D 02        BSR $C0F0  Branch
C0EE: 20 2B        BRA $C11B  Jump
```

We call the subroutine at `$C0F0` twice, each time with different arguments. In a language like C it might look like this:

```
func_C0F0(4, 1);
func_C0F0(12, 0);
```

The function has the following code:

```
C0F0: 34 06        PSHS ,B,A  Push A & B to stack
C0F2: 8E 25 19     LDX #$2519 Set X = $2519
C0F5: BD DB 1B     JSR $DB1B  Call random_next()
C0F8: 81 40        CMPA #$40  A - 64
C0FA: 24 F9        BCC $C0F5  Call random_next() until A < 64
C0FC: 1F 89        TFR A,B    Set B = A
C0FE: 58           ASLB       B * 2
C0FF: 58           ASLB       B * 2
C100: 5C           INCB       B + 1
```

Much like before, we first ensure that we get a random number less than 64, and we then multiply it by four. This time the important difference is that we also add 1. This gives us a random number in the set:

<div>$$
\{ 1, 5, 9, ..., 253 \}
$$</div>

```
C101: 4F           CLRA       Set A = 0
C102: 6D 8B        TST D,X    Examine byte at (X + D)
C104: 26 EF        BNE $C0F5  Call random_next() until byte > 0
```

Again, we use this random number as an offset against the base address of `$2519`, which we stored in register X. This means that we'll be checking the byte at one of the memory addresses in this set:

<div>$$
\{ \mathtt{$2520}, \mathtt{$2524}, \mathtt{$2528}, ..., \mathtt{$2616} \}
$$</div>

If the byte at that location has already been written to, we try it all over again with a new random number.

```
C106: 3A           ABX        Set X = X + B
C107: 86 FF        LDA #$FF   Set A = $FF
C109: A7 84        STA ,X     Store $FF at X
```

Once we've found our random location that's free, we write a fixed value, `$FF`.

```
C10B: BD DB 1B     JSR $DB1B   Call random_next()
C10E: A7 01        STA +$01,X  Store random A at (X + 1)
```

We then get a second random number. This one has no special restrictions, like needing to be a multiple of four; we just take the number by itself, which will be in the range 0---255, and immediately write it to the next memory location. For example, if we just wrote `$FF` to `$2528`, we now write a new random number to `$2529`.

```
C110: A6 61        LDA +$01,S  Set A = stack value B
C112: A7 02        STA +$02,X  Store A at (X + 2)
```

Now we write a third value in the next byte again; continuing our previous example, we would be writing to `$2530`. This time the number is not random, but comes from the stack. We don't change the stack, we just take a peek at it - specifically, we look at the value that we originally loaded into register B at `$C0E4` and `$C0EA` (hence, why I have called it "stack value B" in the comments). Accordingly, this value will be either a 1 or a 0.

```
C114: 6A E4        DEC ,S      stack value A - 1
C116: 26 DA        BNE $C0F2   Loop until stack value A = 0
```

Now we decrement the value in the stack that we first loaded in `$C0E2` and `$C0E8` - either a 4 or a 12 - and loop until it's exhausted.

In summary:

 - the bulk of the subroutine will loop 16 times
 - for each of those 16 loops, 3 bytes will be written to a semi-random location. The bytes are, consecutively:
   - `$FF`
   - a random number between 0 and 255
   - a `1` or a `0`

```
C118: 32 62        LEAS +$02,S Restore stack
C11A: 39           RTS         Return
```

So as to avoid bugs, we clean up the stack pointer by adding 2 (to account for pushing 2 bytes, one each for register A and B, to the stack) and return.