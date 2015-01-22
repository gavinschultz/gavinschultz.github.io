---
layout: post
title: How video display works for the CoCo
---

Exactly how much RAM is used for video, and what does the data translate to on-screen? This particular VDG, the MC6847, provides a resolution of 256 x 192, for a total of 49,152 (48K) pixels; at maximum resolution with one bit per pixel, this means a maximum of 6K of RAM for video. The VDG therefore has 14 modes of varying colors and resolutions:

### Alphanumeric and "semi-graphics" modes
These aren't of much interest to us as they mostly display text and very crude graphical blocks.

 - ALPHANUMERIC INTERNAL - 32 x 16 characters, 2-color, each byte is an ASCII character. Max 0.5K RAM.
 - ALPHANUMERIC EXTERNAL - 32 x 16 characters, 2-color, each byte is a custom character. Max 0.5K RAM.
 - SEMIGRAPHICS FOUR - 64 x 32, 8-color. Max 0.5K RAM.
 - SEMIGRAPHICS SIX - 64 x 48, 4-color. Max 0.5K RAM.

### Resolution modes
These aren't of much interest for games either as you only get 2 colors. Some of the vector-game ports might be nice candidates; I remember a great black-and-white port of Lunar Lander. (Side note: Greg Zumwalt, who coded Starblaze, himself did a Lunar Lander port, but looking at screenshots online it's not the same one I played.

 - RESOLUTION GRAPHICS ONE - 128 x 64, 2-color. Max 1K RAM.
 - RESOLUTION GRAPHICS TWO - 128 x 96, 2-color. Max 1.5K RAM.
 - RESOLUTION GRAPHICS THREE - 128 x 192, 2-color. Max 3K RAM.
 - RESOLUTION GRAPHICS SIX - 256 x 192, 2-color. Max 6K RAM.

### Color modes
Much more interesting as you get 4 colors to play with.

 - COLOR GRAPHICS ONE - 64 x 64, 4-color. Max 1K RAM.
 - COLOR GRAPHICS TWO - 128 x 64, 4-color. Max 2K RAM.
 - COLOR GRAPHICS THREE - 128 x 96, 4-color. Max 3K RAM.
 - COLOR GRAPHICS SIX - 128 x 192, 4-color. Max 6K RAM.

Most games on cartridge that I remember would have used CG6 i.e. the maximum resolution. I guess the only reason you wouldn't is to support those with only 4K of RAM.

The graphics mode is controllable . It also has to be set in the SAM, via a 3-bit value addresses 0xFFC0 to 0xFFC5.
**I haven't figured out how all of the VDG modes are set; the 3-bit address in the SAM is not enough to specify all of the individual modes)

![MC6883 VDG Mode and Display Offset memory mappings](images/MC6883_VDG_Mode_and_Display_Offset_FFC0_to_FFD5.png)

This mapping occurs via the MC6883 Synchronous Address Multiplexer (SAM), which as far as I understand allows common RAM to be used for inputs/outputs, most notably video display. It cleverly does this without interfering with CPU memory accesses because all CPUs compatible with the SAM are known to only access memory in the latter half of the machine cycle. The video display generator (VDG) - typically the MC6847 - can therefore access the common RAM in the first half of the machine cycle.

![MC6883 Interleaved DMA](images/MC6883_Interleaved_DMA.png)

To facilitate this interleaving, the oscillator on the SAM provides the clock for both the CPU and the VDG.