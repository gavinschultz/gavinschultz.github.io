---
layout: post
title:  New Game entry point ($001F—0020)
---

```
C01F: 1A 50        ORCC #$50  Disable hardware interrupts
```

This is identical to `$0000`, which disabled all hardware interrupts (i.e. disabled all keyboard/joystick inputs). Nothing new there, but `$0000` has just been run; the more interesting question is, why are we repeating it? 

To me it suggests that this is a subroutine entry point which we'll jump back to later, from a point at which hardware interrupts are probably *not* disabled. If we search the code for `$001f` we see that this is true:

```
C3AB: 16 F6 E3     LBRA $001F	Branch to $001F
```

By running the game in the debugger, we can see that `$001F` is executed when:

 1. We first turn on or reset the machine (via execution from `$0000`)
 2. We start a new game by selecting a skill level from the start screen (via branching from `$0939`)
