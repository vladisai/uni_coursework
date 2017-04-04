all:
	nasm -f bin loop.asm -o loop 

run:
	qemu-system-x86_64 -drive format=raw,file=loop
