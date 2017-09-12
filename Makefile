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
	git add $(C_SOURCES)
	git add $(S_SOURCES)
	git commit -m "update"
	git push
