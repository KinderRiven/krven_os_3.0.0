#Makefile

C_SOURCES = $(shell find . -name "*.c")
S_SOURCES = $(shell find . -name "*.s")

CC = GCC
LD = ld
ASM = nasm
AS = as

all: boot setup floppy

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
	git commit -m "update"
	git push
