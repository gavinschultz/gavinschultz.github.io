---
layout: post
title:  Initialize player starting sector ($C11B—C12C)
---

```
C11B: BD DB 1B     JSR $DB1B  Call random_number()
C11E: 81 40        CMPA #$40  A - 64?
C120: 24 F9        BCC $011B  Call random_number() while A > 63
C122: 97 2A        STA $2A    Store A in $202A
```

We're again generating a random number in the range 0---63, which suggests another placement into a sector of the galaxy map. Only a single position is generated, and the only thing that we *haven't* placed on the map is the player themselves; this is the initial sector into which they are spawned. We will in future refer to the data at `$202A` as the variable `player_sector_number`.

```
C124: 1F 89        TFR A,B    Copy A -> B
C126: 58           ASLB       B = B * 2
C127: 58           ASLB       B = B * 2
C128: 8E 25 19     LDX #$2519 Set X = $2519
C12B: 3A           ABX        X = X + B
C12C: 9F 36        STX $36    Store X in $2036
```

Using the same sector number, we generate an offset into galaxy map memory, almost exactly as we did for the alien population in `$C0B2-C0C0`. 

For example, suppose the player is randomly chosen to start in sector 10 (`$0A`). From this we create an address offset:

$$
\begin{array}{l@{\,}l}
X &= \mathtt{$2519} + (10\times2\times2) \\
&= \mathtt{$2541}
\end{array}
$$

But this time we store the address itself (e.g. `$2541`) at `$2036`. In other words, we are tracking the player's position in the galaxy, but it's not recorded in the galaxy map itself. Rather we can say we've created a variable which points to the relevant galaxy sector. Such a variable can in fact be called a *pointer* and we will refer to it with an asterix prefix as the `*player_sector`.

## Deriving galaxy map properties
TODO: the derivation of properties fits here but the discussion of property syntax and addresses vs values does not.
Note in our comments that we refer first to X as the **player_location**, but we then immediately refer to the value at that location as **player_location.alien_count**. This is because:

 1. As a memory address (e.g. `$2519`), it tells us which sector of the galaxy the player is in
 2. The *value* at that address (e.g. `19`) is the number of aliens in that sector.

That's why we use the term **player_location.alien_count**; we're using the value at the player_location address, which is the number of aliens in the sector.

We can derive this and other properties of the sector by offsetting from the player_location address and comparing to what's shown on the sector map. 
TODO: diagram, overlay actual initialized map onto memory map to show correlation.

 - `player_location` + 0 = `player_location.alien_count`
 - `player_location` + 1 = `player_location.station_health`
 - `player_location` + 2 = `player_location.???` (initialized to a random number 0-255)
 - `player_location` + 3 = `player_location.station_type`

Each sector uses only four bytes, so player_location + 4 puts us in the first byte of the next sector, so the value will be the alien count of the *next* sector.
