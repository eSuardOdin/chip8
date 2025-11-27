# chip8
Create an emulator 101


https://github.com/Timendus/chip8-test-suite
gcc chip8.c display.c main.c opcodes.c keyboard.c -o chip8 $(sdl2-config --cflags --libs)
