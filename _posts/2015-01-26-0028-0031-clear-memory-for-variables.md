---
layout: post
title:  Clear memory for variables ($0028—0031)
---

```
0028: 8E 20 00     LDX #$2000   Set X to address 0x2000
002B: 6F 80        CLR ,X+      Set memory at X to 0
002D: 8C 26 F9     CMPX #$26F9  Check if we've reached 0x26F9
0030: 26 F9        BNE $002B    Keep going if not
```

This zeroes all the memory between 0x2000 and 0x26F9. Perhaps this is where we'll be storing all of our variables and other data?

Not that it's going to make much difference, but I wonder why we don't set 2 bytes at a time like we did when [clearing the screen]({% post_url 2015-01-25-0025-0027-clear-the-screen %})?

i.e.

```
LDD #$0000   Load D with 0000
STD ,X++     Store 0000 to memory
```

instead of
```
CLR ,X+     Clear to memory (i.e. store 00)
```

Possibly because we're dealing with an odd number of bytes here, so if we set two at a time we need to add another instruction at the end to set just one.

Or maybe the routine when clearing the screen was a copy-and-paste from elsewhere, hence just someone else's coding style.
