TOP = .

all: os.img

CC 		:= gcc
LD 		:= ld
QEMU	:= qemu-system-i386
GDB		:= gdb

OBJ_DIR			:= obj
OBJ_LIB_DIR		:= $(OBJ_DIR)/$(LIB_DIR)

LIB_C := $(shell find $(LIB_DIR) -name "*.c")
LIB_O := $(LIB_C:%.c=$(OBJ_DIR)/%.o)


CFLAGS := -Wall -Werror -Wfatal-errors #开启所有警告, 视警告为错误, 第一个错误结束编译
CFLAGS += -MD #生成依赖文件
CFLAGS += -std=gnu11 -m32 -c #编译标准, 目标架构, 只编译
#CFLAGS += -I . #头文件搜索目录
CFLAGS += -O0 #不开优化, 方便调试
CFLAGS += -fno-builtin #禁止内置函数
CFLAGS += -ggdb3 #GDB调试信息

ASFLAGS := -m32 -MD
LDFLAGS := -melf_i386
QEMU 	:= qemu-system-i386

include config/Makefile.build
include config/Makefile.git

include boot/Makefile.part
include kern/Makefile.part
include game/Makefile.part
#include lib/Makefile.part

os.img: bootblock kern game
	cat $(BOOT) $(KERN) $(GAME) > obj/os.img
	$(call git_commit, "compile", $(GITFLAGS))

IMAGES	:= $(OBJ_DIR)/os.img
GDBPORT := $(shell expr `id -u` % 5000 + 25000)
QEMUOPTS = $(OBJ_DIR)/os.img -serial mon:stdio
QEMUOPTS += $(shell if $(QEMU) -nographic -help | grep -q '^-D '; then echo '-D qemu.log'; fi)
QEMUOPTS += -monitor telnet:127.0.0.1:1111,server,nowait
#QEMUOPTS += $(QEMUEXTRA)

.PHONY: clean debug gdb display submit commit log

display:
	@echo $(GDBPORT)

.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@
pre-qemu: .gdbinit

gdb:
	gdb -x .gdbinit

qemu: os.img pre-qemu
	$(QEMU) $(QEMUOPTS)
	$(call git_commit, "run qemu", $(GITFLAGS))

debug: $(IMAGES) pre-qemu
	$(QEMU) -s $(QEMUOPTS) -S 

clean: 
	rm -rf obj/*

submit: clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2

commit:
	@git commit --allow-empty

log:
	@git log --author=Akuxcw
