TOP = .

all: os.img

CC 		:= gcc
LD 		:= ld
QEMU	:= qemu-system-i386
GDB		:= gdb

OBJ_DIR			:= obj
LIB_DIR			:= lib
BOOT_DIR		:= boot
KERNEL_DIR		:= kernel
GAME_DIR		:= game
OBJ_LIB_DIR		:= $(OBJ_DIR)/$(LIB_DIR)
OBJ_BOOT_DIR	:= $(OBJ_DIR)/$(BOOT_DIR)
OBJ_KERNEL_DIR	:= $(OBJ_DIR)/$(KERNEL_DIR)
OBJ_GAME_DIR	:= $(OBJ_DIR)/$(GAME_DIR)

CFLAGS := -Wall -Werror -Wfatal-errors #开启所有警告, 视警告为错误, 第一个错误结束编译
CFLAGS += -MD #生成依赖文件
CFLAGS += -std=gnu11 -m32 -c #编译标准, 目标架构, 只编译
CFLAGS += -I . #头文件搜索目录
CFLAGS += -O0 #不开优化, 方便调试
CFLAGS += -fno-builtin #禁止内置函数
CFLAGS += -ggdb3 #GDB调试信息

ASFLAGS := -m32 -MD
LDFLAGS := -melf_i386
QEMU 	:= qemu-system-i386

CFILES 	:= $(shell find game/ lib/ -name "*.c")
SFILES 	:= $(shell find game/ lib/ -name "*.S")
OBJS 	:= $(CFILES:.c=.o) $(SFILES:.S=.o)

include config/Makefile.build
include config/Makefile.git

include boot/Makefile.part
#include game/Makefile.part
#include lib/Makefile.part

os.img: game bootblock
	cat obj/boot/bootblock obj/game/game > obj/os.img
	
game: $(OBJS)
	@mkdir -p obj/game
	$(LD) $(LDFLAGS) -e game_init -Ttext 0x00100000 -o obj/game/game $(OBJS)
	$(call git_commit, "compile game", $(GITFLAGS))

-include $(patsubst %.o, %.d, $(OBJS))

IMAGES	:= $(OBJ_DIR)/os.img
GDBPORT := $(shell expr `id -u` % 5000 + 25000)
QEMUOPTS = $(OBJ_DIR)/os.img -serial mon:stdio
QEMUOPTS += $(shell if $(QEMU) -nographic -help | grep -q '^-D '; then echo '-D qemu.log'; fi)
#QEMUOPTS += $(QEMUEXTRA)

.PHONY: play clean debug gdb

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

clean: clean-mdr
	rm -f obj/game/game obj/os.img $(OBJS) $(OBJS:.o=.d)
