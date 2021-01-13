all : runtime.o

runtime.o: runtime.asm
	nasm -felf64 runtime.asm

clean:
	rm -f runtime.o
