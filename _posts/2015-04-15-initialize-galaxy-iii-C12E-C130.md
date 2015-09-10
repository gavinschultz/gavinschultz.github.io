---
layout: post
title:  Initialize galaxy III ($C12E—C130)
---

```
C12E: BD D3 79   JSR $D379   Jump
```

```
D379: D6 2A      LDB $2A   Set B = player_sector_number
D37B: D7 29      STB $29   Store player_sector_number to $2029
```

First we take the player's current sector - a number between 0 and 63 which indicates their current square on the map - and copies it to an adjacent byte in memory. A temporary variable?

```
D37D: 96 29      LDA $29   Set A = player_sector_number copy
D37F: 4C         INCA      A = A + 1
D380: FD 26 FA   STD $26FA Set $26FA = A + 1, $26FB = A
D383: FD 26 FC   STD $26FC Set $26FC = A + 1, $26FD = A
```

The copied player sector - incremented by 1 - is now placed in the accumulator. We appear to be using it to reset all of the values used internally by the random_number() function. For example, if the player is currently in sector 7, the random number generator bytes are reset to `08070807`.

There's no obvious mathematical outcome of this, but messing with the internals like this re-seeds the random number generator. The intent may be to provide more guaranteed "randomness" for initialization - as we saw, the random number generator is not particularly good in its initial cycles. A limitation of this is that, since the player position can only be one of 64 possible values, the generator can only be reseeded in 64 unique ways.

```
D386: 9E 36        LDX $36    Set X = player_sector
D388: 27 06        BEQ $D390  If player_sector = null, go to $D390
D38A: 96 35        LDA $35    A = value at $2035
D38C: AB 84        ADDA ,X    A = A + player_sector->alien_count
D38E: A7 84        STA ,X     player_sector->alien_count = A
```

The code from `$D38A` is skipped if the player's current sector is not 0; perhaps this happens when the player is dead. We add the value at `$2035` to the number of aliens in the player's sector. As this is the first time we've used `$2035`, we can't say yet what it might be used for. (Looking ahead, it will be set once we reach `$D3CA`.)

```
D390: 96 6F        LDA $6F    Set A = value at $206F
D392: A7 01        STA +$01,X Store A at player_sector->station_health
```

Now we transfer the value at `$206F` to the second byte of the player's sector, which stores the station health of any station in the sector (or `0` if there's no station at all). Again, we don't know what `$206F` is used for yet, but at least initially it's set to zero, destroying any station in the player's current sector.

```
D394: D6 29        LDB $29    Set B = player_sector_number copy
D396: 58           ASLB       B = B * 2
D397: 58           ASLB       B = B * 2
D398: 8E 25 19     LDX #$2519 Set X = galaxy map
D39B: 3A           ABX        X = galaxy map + (player_sector * 4)
D39C: 9F 36        STX $36    Store X in player_address
```

We recalculate the address of the player's sector based on the player's sector number and rewrite the value into the `player_sector` pointer. During initialization, this doesn't do anything because the `player_sector` already reflects the sector number; however, this function is *also* called when a user warps to a new sector, at which time the `player_sector_number` does not correspond to the `player_sector`. The code above therefore synchronizes the two.

``` 
D39E: BD D1 1D     JSR $D11D   initialize_unknown()
```

### Initialize playfield aliens

```
D3A1: 9E 36        LDX $36     X = player_sector
D3A3: A6 84        LDA ,X      A = player_sector->alien_count
D3A5: B1 27 00     CMPA $2700  player_sector->alien_count - skill_level
D3A8: 23 0A        BLS $D3B4   Branch if player_sector->alien_count < skill_level
```

```
D3AA: B0 27 00     SUBA $2700  A = A - skill_level
D3AD: A7 84        STA ,X      Store player_sector->alien_count = A
D3AF: B6 27 00     LDA $2700   A = skill_level
D3B2: 20 02        BRA $D3B6   
```

The above code only runs when the number of aliens in the player's sector is higher than the current skill level. In this case the number of aliens is reduced by the player's skill level.

```
D3B4: 6F 84        CLR ,X      Set X = 0
```

This code is only executed when the alien count is less than the skill level, in which case the number of aliens is set to zero.

```
D3B6: 97 35        STA $35     $2035->val = A (skill_level or sector total alien count, 0 if no aliens)
D3B8: 1F 89        TFR A,B     Set B = A
D3BA: BD D1 CF     JSR $D1CF   
```

```
D1CF: 8E 20 81     LDX #$2081   X = $2081
D1D2: 34 04        PSHS ,B      Push B to stack (temp1)
D1D4: CC 00 00     LDD #$0000   A = 0, B = 0
D1D7: A7 02        STA +$02,X   (X+2)->val = 0
D1D9: E7 04        STB +$04,X   (X+4)->val = 0
D1DB: 30 88 10     LEAX +$10,X  X = X + 16
D1DE: 8C 21 01     CMPX #$2101  X - $2101
D1E1: 26 F4        BNE $D1D7    Loop until X = $2101 (8 loops)
```
More data initialization. It seems that from `$2081` is probably a data structure with 8 properties of 16-bytes each, but we don't know what it is yet.

```
D1E3: 8E 20 81     LDX #$2081   X = $2081
D1E6: E6 E4        LDB ,S       B = temp1
D1E8: C1 00        CMPB #$00    Check B = 0
D1EA: 27 20        BEQ $D20C    Branch if B = 0
```

The above will branch if the number of aliens in the player's sector is 0.

```
D1EC: BD DB 1B     JSR $DB1B    Call random_number()
D1EF: 27 FB        BEQ $D1EC    Loop if random number = 0
D1F1: 81 84        CMPA #$84    A - $84 (132)
D1F3: 22 F7        BHI $D1EC    Loop if random number > 134
D1F5: A7 02        STA +$02,X   (X+2)->val = A (random number < 134)
```

We generate a random number and keep doing so until we get a number less than 134. We then store that value in memory. TODO: why that specific number?

```
D1F7: BD DB 1B     JSR $DB1B    Call random_number()
D1FA: A7 04        STA +$04,X   (X+4)->val = A (random number)
D1FC: 86 01        LDA #$01     A = 1
D1FE: A7 07        STA +$07,X   (X+7)->val = 1
D200: 30 88 10     LEAX +$10,X  X = X + 16
D203: 8C 21 01     CMPX #$2101  X - $2101
D206: 27 04        BEQ $120C    Loop until X = $2101 (8 loops)
D208: 6A E4        DEC ,S       Decrement temp1
D20A: 26 E0        BNE $D1EC    Keep looping until temp1 = 0
```

```
D20C: 32 61        LEAS +$01,S  Reset stack, destroy temp1
D20E: 39           RTS
```

```
D3BD: 9E 36        LDX $36      X = player_sector
D3BF: A6 01        LDA +$01,X   A = player_sector->station_health
D3C1: 26 07        BNE $D3CA    Branch if player_sector->station_health > 0
D3C3: 0F 6B        CLR $6B      $206B = 0
D3C5: 0F 6C        CLR $6C      $206C = 0
D3C7: 0F 6F        CLR $6F      $206F = 0
D3C9: 39           RTS
```

### Initialize station in sector

```
D3CA: 97 6F        STA $6F     $206F->val = A (player_sector->station_health)
```

The station health is stored in `$206F`.

```
D3CC: 86 80        LDA #$80    A = $80
D3CE: 97 6B        STA $6B     $206B->val = $80 (128)
```

The value `$80`  is stored in `$206B`.

```
D3D0: A6 02        LDA +$02,X  A = player_sector->??? (random number)
D3D2: 97 6C        STA $6C     $206C->val = A
```

The (still unknown) third byte is stored in `$206C`.

```
D3D4: 6D 03        TST +$03,X  test player_sector->station_type
D3D6: 26 05        BNE $D3DD   Skip next 2 lines if X->val != 0
D3D8: CE DD 1E     LDU #$DD1E  U = $DD1E
D3DB: 20 03        BRA $D3E0   Skip next line
D3DD: CE DC DE     LDU #$DCDE  U = $DCDE
D3E0: DF 6D        STU $6D     $206D->val = U
```

First use of the user stack (U) rather than the hardware stack (S). Depending on the station type, `$206D` will store one of two ROM addresses. These are probably going to point to the station images bytes.

```
D3E2: 39           RTS
```