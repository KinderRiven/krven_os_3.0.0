#Makefile

C_SOURCES = $(shell find . -name "*.c")
S_SOURCES = $(shell find . -name "*.s")
FILES = floppy.img bochsrc.txt Makefile KERNEL

CC = gcc
LD = ld
ASM = nasm
AS = as

all: tools boot setup floppy

.PHONY:tools
tools:
	$(CC) tools/build.c -o tools/build	

.PHONY:bochs
bochs:
	bochs -f bochsrc.txt
.PHONY:boot
boot:
	$(ASM) boot/boot.s -o boot/boot.o

.PHONY:setup
setup:
	$(ASM) boot/setup.s -o boot/setup.o

.PHONY:git_pull
git_pull:
	git pull

.PHONY:floppy
floppy:
	./tools/build boot/boot.o boot/setup.o > KERNEL
	dd if=KERNEL of=floppy.img conv=notrunc

.PHONY:git_push
git_push:
	git add $(C_SOURCES)
	git add $(S_SOURCES)
	git add $(FILES)
	git commit -m "update"
	git push
