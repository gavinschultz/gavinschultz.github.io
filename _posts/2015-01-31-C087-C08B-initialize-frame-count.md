---
layout: post
title:  Initialize frame count ($C087—C08B)
---

```
C087: CC C0 00     LDD #$C000   Set D = 49152 ($C000)
C08A: DD 67        STD $67      Store to $2067
```

The 2-byte variable at `$2067` reveals itself once we observe it with the game in motion. Each time a VSYNC occurs, i.e. for each frame, it increments by one. Therefore, it seems reasonable to call this variable **frame_count**.

It starts at `$C000`, but once it reaches `$DFFF` (after 8,191 frames) it resets back to `$C000` and starts again.
