#!/bin/bash
nasm -f bin ../src/boot.asm -o ../bin/boot.bin
ndisasm ../src/boot.asm -o100h > ../bin/boot.log
