---
layout: post
title:  Initialize unknown variables $2029—$202A ($008C—008F)
---

```
008C: 0F 29        CLR $29      Set $2029 = 0
008E: 0F 2A        CLR $2A      Set $202A = 0
```

Here's our first instance of direct addressing; since we already set the offset to 0x2000, the `CLR $39` actually means `CLR$2039`. I presume one or two variables will be stored here, though we don't yet know what they are.