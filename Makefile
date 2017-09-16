#Makefile
CC = gcc
LD = ld
ASM = nasm
AS = as

CC_FLAGS = -c -Wall -m32 -nostdinc -Iinclude
LD_FLAGS = -m elf_i386 -T script/kernel.ld -nostdlib

all: image

.PHONY: image
image: tools/build boot/boot boot/setup
	./tools/build boot/boot boot/setup > FLOPPY
	dd if=FLOPPY of=floppy.img conv=notrunc

tools/build: tools/build.c
	$(CC) tools/build.c -o tools/build

boot/boot: boot/boot.s
	$(ASM) boot/boot.s -o boot/boot

boot/setup: boot/setup.s 
	$(ASM) boot/setup.s -o boot/setup

.PHONY:kernel
kernel: boot/head.o init/main.o

#file.c:file.o
.c.o:
	$(CC) $(CC_FLAGS) $< -o $@

#file.s:file.o
.s.o:
	$(ASM) $< -o $@

.PHONY:bochs
bochs: bochsrc.txt
	bochs -f bochsrc.txt

.PHONY:clean
clean:
	rm -f $(shell find . -name "*.o") tools/build boot/boot boot/setup

#github
.PHONY:git_pull
git_pull:
	git pull

.PHONY:git_push
git_push:
	git add $(shell find . -name "*.s")
	git add $(shell find . -name "*.c")
	git add floppy.img bochsrc.txt Makefile
	git commit -m "$(shell date)"
	git push
