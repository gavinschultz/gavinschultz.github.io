---
layout: post
title:  Global variables ($00—00)
---

```
03FF: stack point (initialized at $C015)
2029: unknown (initialized to 0 at $C08C)
202A: unknown (initialized to 0 at $C08E)
2039: unknown (initialized to 0 at $C04B)
2045: unknown skill-based variable (initialized at $C039)
2047: unknown skill-based variable (initialized at $C03B)
2055: unknown (initialised to 0 at $C085)
2065: unknown skill-based variable (initialized at $C080)
2067: frame count (initialized to C000 at C08A)
26FA: random number seed byte 1 (initialized to 1 at $C013)
26FB: random number seed byte 2 (initialized to 1 at $C013)
26FC: random number seed byte 3 (initialized to 1 at $C013)
26FD: random number seed byte 4 / last random number used (initialized to 1 at $C013) - this byte copied to the accumulator when random_number() is called
26FE: unknown (initialized to 1 at $C007)
26FF: unknown (initialized to 1 at $C00B)
2700: skill level (initialized to 8 at $C00A)
```