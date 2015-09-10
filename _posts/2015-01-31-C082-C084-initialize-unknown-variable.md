---
layout: post
title:  Initialize unknown variable ($C082—C084)
---

```
C082: CC 0A 8C     LDD #$0A8C   Set D = 2700 ($0A8C)
C085: DD 55        STD $55      Store to $2055
```

I can't find anywhere that reads data from $2055; I don't know what the point of the variable at that location is.
