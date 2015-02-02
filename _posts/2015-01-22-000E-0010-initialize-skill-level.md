---
layout: post
title:  Initialize skill level ($000E—0010)
---

```
C00E: 86 08        LDA #$08    
C00A: B7 27 00     STA $2700   Store 8 in $2700
```

Via experimentation with the debugger, we can observe that the value at `$2700` appears to be set to:

 - 8 when we start/reset the machine and load the start screen
 - 1 when the user starts a game with skill level 1 (easiest)
 - 2 when the user starts a game with skill level 2
 - 3 when the user starts a game with skill level 3
 - 4 when the user starts a game with skill level 4
 - 5 when the user starts a game with skill level 5
 - 6 when the user starts a game with skill level 6
 - 7 when the user starts a game with skill level 7
 - 8 when the user starts a game with skill level 8 (hardest)

It's fairly obvious, then, that the variable at `$2700` indicates the skill level. Most likely we initialize it to 8, the hardest skill level, so that the aliens running in "demo mode" on the start screen look more aggressive.
