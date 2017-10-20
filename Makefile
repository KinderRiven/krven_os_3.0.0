#Makefile
CC = gcc
LD = ld
ASM = nasm
AS = as

CC_FLAGS = -c -Wall -m32 -nostdinc -Iinclude -fno-builtin -fno-stack-protector
LD_FLAGS = -m elf_i386 -T script/kernel.ld -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: image

.PHONY: image
image: tools/build boot/boot boot/setup kernel
	./tools/build boot/boot boot/setup KERNEL.bin > FLOPPY
	dd if=FLOPPY of=floppy.img conv=notrunc

.PHONY: tools/build
tools/build: tools/build.c
	$(CC) tools/build.c -o tools/build

.PHONY: boot/boot
boot/boot: boot/boot.s
	$(ASM) boot/boot.s -o boot/boot

.PHONY: boot/setup
boot/setup: boot/setup.s 
	$(ASM) boot/setup.s -o boot/setup -I boot/

.PHONY:kernel
kernel: boot/kernel.o init/main.o \
	kernel/system.o kernel/sched.o \
	kernel/traps.o kernel/traps_s.o \
	kernel/system_call.o kernel/system_call_s.o \
	kernel/io.o lib/string.o kernel/console.o  kernel/asm.o \
	mm/memory.o \
	kernel/fork.o \
	kernel/vsprintf.o kernel/printk.o
	$(LD) $(LD_FLAGS) \
	boot/kernel.o init/main.o \
	kernel/system.o  kernel/sched.o \
	kernel/traps.o kernel/traps_s.o \
	kernel/system_call.o kernel/system_call_s.o \
	kernel/io.o lib/string.o kernel/console.o kernel/asm.o \
	mm/memory.o \
	kernel/fork.o \
	kernel/vsprintf.o kernel/printk.o \
	-o KERNEL
	objcopy -O binary KERNEL KERNEL.bin

#file.c:file.o
.c.o:
	$(CC) $(CC_FLAGS) $< -o $@

#file.s:file.o
.s.o:
	$(ASM) $(ASM_FLAGS) $< -o $@

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
git_add:
	git add $(shell find . -name "*.s")
	git add $(shell find . -name "*.c")
	git add floppy.img bochsrc.txt Makefile
