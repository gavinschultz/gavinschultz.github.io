---
layout: post
title:  Initialize stack ($C021—C024)
---

```
C015: 10 CE 3F FF  LDS #$3FFF  Set stack pointer to 0x3FFF
```

This instruction sets the memory location for our stack to 0x3FFF. Choosing this particular address is somewhat arbitrary; it just needs to be somewhere that won't be used by anything else.

## What is the stack?

The stack can be used for a couple of different things.

### Call stack for jumps and returns
If we peek ahead at the next section we'll see that the next command is a `JSR` (Jump to Subroutine), which will cause execution to jump immediately to a given memory address. It's important, however, that when we want to return from that address, we know where to go back to. The stack will automatically do this for us, and this feature is known as a *call stack*. For example:

 - following the `JSR` (Jump To Subroutine) command, the address `0028` will be automatically stored at 0x3FFD, and the stack register S will point to 0x3FFD.
 - following the next `RET` (Return) command, execution will return to 0x0028 and the stack register S will point to 0x3FFF again.

### Temporary data storage


Notice that the stack grows *downwards*, so 0x3FFF points to the top of the stack, and anything added to the stack is added at memory addresses below that.

