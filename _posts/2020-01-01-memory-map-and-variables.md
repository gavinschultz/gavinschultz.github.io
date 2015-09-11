---
layout: post
title:  Memory map and variables ($00—00)
---

```
0600 - 1E00: video memory
1CF8 - 1DEF: unknown data structure (initialed from $C135)
2029: player_sector_number copy? (initialized to same as player_sector at $D37B)
202A: player_sector_number (initialized to 0 at $C08E)
2035: unknown (added to alien_count at $D38A)
2036: *player_sector (initialized at $C12C)
2039: unknown (initialized to 0 at $C04B)
2045: unknown skill-based variable (initialized at $C039)
2047: unknown skill-based variable (initialized at $C03B)
2051: unused variable? (initialized to 3 at $C133)
2055: unknown (initialised to 0 at $C085)
2065: unknown skill-based variable (initialized at $C080)
2067: frame count (initialized to C000 at C08A)
2081 - 2101: aliens on playfield (initialized from $D1CF)
2129 - 22E2: unknown data structure (initialized from $D11D)
2519 - 2618: galaxy map, 64 sectors of 4 bytes each
 - byte 0 = alien_count
 - byte 1 = station_health
 - byte 2 = ??? (initialized to a random number 0-255)
 - byte 3 = station_type
26FA: random number seed byte 1 (initialized to 1 at $C013)
26FB: random number seed byte 2 (initialized to 1 at $C013)
26FC: random number seed byte 3 (initialized to 1 at $C013)
26FD: random number seed byte 4 / last random number used (initialized to 1 at $C013) - this byte copied to the accumulator when random_number() is called
26FE: unknown (initialized to 1 at $C007)
26FF: unknown (initialized to 1 at $C00B)
2700: skill level (initialized to 8 at $C00A)
3FFF: initial stack pointer (initialized at $C021)
```