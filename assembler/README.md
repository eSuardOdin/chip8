# CHIP 8 Assembler

Le but est de transformer un code "humainement lisible" en langage assembleur CHIP8.

Ainsi, un code comme :

```
:main

LD v8, 0x5b 	# Load la valeur dans le registre 8
SNE v8, v5		
CALL equalize
LD I, 0x33

:equalize
LD v5, 0x5b
RET
```

En :
```
685B
9850
2004
A033
655B
00E0
```


