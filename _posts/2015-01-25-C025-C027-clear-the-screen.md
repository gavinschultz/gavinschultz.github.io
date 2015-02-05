---
layout: post
title:  Clear the screen ($C025—C027)
---

```
C025: BD D7 DC     JSR $17DC   Jump
```

The subroutine at $17DC looks like this:

```
D7DC: 8E 06 00     LDX #$0600
D7DF: CC 00 00     LDD #$0000
D7E2: ED 81        STD ,X++ 
D7E4: 8C 1E 00     CMPX #$1E00
D7E7: 26 F9        BNE $17E2
D7E9: 39           RTS
```

Huh. So this basically starts at address `$0600` - which we established was the start of video memory - and zeroes out all the bytes until `$1E00`. Funny, I always thought that zeroing out memory could be done in a single bulk operation somehow, but nope, we have to loop 3,072 times, zeroing out 2 bytes at a time.

It's no coincidence that we're zeroing out exactly 6K of RAM. We previously established that the video RAM would start at `$0600`, and that the COLOR GRAPHICS SIX mode would use 6K of RAM.

In other words, we've turned the whole screen green.

## Why is my screen not completely green?
Interestingly, even after doing this, if we pause execution and look at the screen there are some random artifacts visible:

![Random artifacts after screen clear](../images/Random_artifacts_after_clear_video_0025.png)

Unlike modern LCD monitors, CRT televisions and monitors have an electron gun that fires horizontal beams at the screen, moving back and forth from top-to-bottom. The artifacts that we see are only at the bottom of the screen, which suggests they're remnants from the previous state of the video RAM, from before the memory was cleared. They're still visible simply because the electron gun has not quite reached those scanlines yet. Give the screen a few milliseconds to refresh and it will disappear.
