#Makefile

C_SOURCES = $(shell find . -name "*.c")
S_SOURCES = $(shell find . -name "*.s")

CC = GCC
LD = ld
ASM = nasm

all:git_pull


git_pull:
	git pull

git_push:
	#if [ "$(C_SOURCES)" != "" ]; then
	git add $(C_SOURCES)
	#fi
	#if [ "$(S_SOURCES)" != "" ]; then
	git add $(S_SOURCES)
	#fi
	git commit -m "update"
	git push
