all:
	nasm -g -f  bin ./src/boot/boot.asm -o ./bin/boot.bin


clean:
	rm -rf ./bin/*
