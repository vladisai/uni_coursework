loop: loop.asm
	nasm -f bin loop.asm -o loop 

jumper: jumper.asm
	nasm -f bin jumper.asm -o jumper 

all: loop jumper 

run:
	./print_name
	#qemu-system-x86_64 -drive format=raw,file=jumper
