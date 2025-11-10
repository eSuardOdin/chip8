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

# CHIP-8 Instructions par mot-clé

- `CLS`
	- No arguments : `00E0`
- `RET`
	- No arguments : `00EE`
- `SYS`
	- SYS *addr* : `0nnn`
- `JP`
	- JP *addr* : `1nnn`
	- JP V0, *addr* : `Bnnn`
- `CALL`
	- CALL *addr* : `2nnn` 
- `SE`
	- SE V*x*, *byte* : `3xkk`
	- SE V*x*, V*y* : `5xy0`
- `SNE`
	- SNE V*x*, *byte* : `4xkk`
	- SNE V*x*, V*y* : `9xy0`
- `LD`
	- LD V*x*, *byte* : `6xkk`
	- LD V*x*, V*y* : `8xy0`
	- LD I, *addr* : `Annn`
	- LD V*x*, DT : `Fx07`
	- LD V*x*, K : `Fx0A`
	- LD DT, V*x* : `Fx15`
	- LD ST, V*x* : `Fx18`
	- LD F, V*x* : `Fx29`
	- LD B, V*x* : `Fx33`
	- LD [I], V*x* : `Fx55`
	- LD V*x*, [I] : `Fx65`
	- LD HF, V*x* : `Fx30` (Super Chip-48)
	- LD R, V*x* : `Fx75` (Super Chip-48)
	- LD V*x*, R : `Fx85` (Super Chip-48)
- `ADD`
	- ADD V*x*, *byte* : `7xkk`
	- ADD V*x*, V*y* : `8xy4`
	- ADD I, V*x* : `Fx1E`
- `OR`
	- OR V*x*, V*y* : `8xy1`
- `AND`
	- AND V*x*, V*y* : `8xy2`
- `XOR`
	- XOR V*x*, V*y* : `8xy3`
- `SUB`
	- SUB V*x*, V*y* : `8xy5`
- `SHR`
	- SHR V*x* {, V*y*} : `8xy6`
- `SUBN`
	- SUBN V*x*, V*y* : `8xy7`
- `SHL`
	- SHL V*x* {, V*y*} : `8xyE`
- `RND`
	- RND V*x*, *byte* : `Cxkk`
- `DRW`
	- DRW V*x*, V*y*, *nibble* : `Dxyn`
	- DRW V*x*, V*y*, 0 : `Dxy0` (Super Chip-48)
- `SKP`
	- SKP V*x* : `Ex9E`
- `SKNP`
	- SKNP V*x* : `ExA1`
- `SCD`
	- SCD *nibble* : `00Cn` (Super Chip-48)
- `SCR`
	- No arguments : `00FB` (Super Chip-48)
- `SCL`
	- No arguments : `00FC` (Super Chip-48)
- `EXIT`
	- No arguments : `00FD` (Super Chip-48)
- `LOW`
	- No arguments : `00FE` (Super Chip-48)
- `HIGH`
	- No arguments : `00FF` (Super Chip-48)


