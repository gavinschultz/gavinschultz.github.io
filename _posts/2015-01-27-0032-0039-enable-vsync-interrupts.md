---
layout: post
title:  Enable VSYNC interrupts ($0032—0039)
---

```
0032: B6 FF 03     LDA $FF03   Load data from 0xFF03
0035: 8A 01        ORA #$01    Set bit 0 (OR on 0000 0001)
0037: B7 FF 03     STA $FF03   Save back to RAM
003A: B6 FF 02     LDA $FF02   Touch 0xFF02 to clear flags
```

Here we're writing a mask to the I/O system via the SAM again, just as we did before [to set the VDG mode]({% post_url 2015-01-23-0013-0015-initialize-video %}). This time it corresponds to the **I/O<sub>0</sub> (Slow)** flags.

![MC6883 I/O memory mapping](../images/MC6883_IO_FF00_to_FF5F.png)

Again, the detailed memory map comes to the rescue:

![CoCo Memory Map at 0xFF03](../images/CoCo_Memory_Map_FF03.png)

So we're enabling "IRQ to CPU" for the field sync. A field sync is the same as a vertical sync (VSYNC); this is the small pause that occurs 60 times a second, once the electron beam has drawn the entire screen (all 525 horizontal lines) and is returning to the upper-left corner. An interrupt signal that it sends at this time is most usefully read as a message from the video controller saying, **"Hey! I've drawn the whole screen but I haven't started on the next one yet; if there are any changes you want to make for the next screen, now might be a good time to do it."**

It might be useful to see what happens to the game if we *don't* allow the CPU to be notified of the VSYNC. To get a better idea of what this does I tried setting the byte at 0x36 from `01` to `00`, which causes the code to do nothing and keeps the IRQ to CPU disabled. What happened was this:

![What happens with IRQ to CPU disabled for vertical sync](../images/disabling_vsync_irq_to_cpu.png)

We get the basics of the HUD displayed, but nothing else. The game appears to be otherwise locked on this screen. I would guess this means that the program is actually waiting for the VSYNC signal before proceeding. It likely does this because writing to the video RAM at the same time that it's being rendered to screen is not usually a great idea (you might end up drawing half of the previous state and half of the new state).

### What's that got to do with 0xFF02?
The final instruction in this snippet is `LDA $FF02`, which appears to make no sense because we don't subsequently do anything with the stored value.

The only information I've been able to find so far - though it's probably formally documented *somewhere*; is in [an online esoteric discussion of the VSYNC interrupt](http://www.coco3.com/community/2010/02/vblank-and-hblank/):

*"As to your question about reading $FF00 or $FF02, that clears the interrupt flag of the respective byte $FF01 or $FF03."*

Fair enough. Having just enabled the VSYNC interrupt, we want to ensure that the interrupt flag (on bit 7 of 0xFF03) is cleared. In MESS I don't actually see the data on 0xFF03 change, nor does skipping this instruction seem to affect the game.