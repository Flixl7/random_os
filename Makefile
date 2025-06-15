NASM = nasm
CC = i386-elf-gcc
LD = i386-elf-ld
QEMU = qemu-system-x86_64

BUILD_DIR = Binaries
BOOT_ASM = boot.asm
KERNEL_ENTRY_ASM = kernel_entry.asm
KERNEL_C = kernel.c
ZEROES_ASM = zeroes.asm

BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_ENTRY_O = $(BUILD_DIR)/kernel_entry.o
KERNEL_O = $(BUILD_DIR)/kernel.o
PRINT_CHAR_O = $(BUILD_DIR)/print_char.o
SHELL_O = $(BUILD_DIR)/simple_shell.o
ZEROES_BIN = $(BUILD_DIR)/zeroes.bin
FULL_KERNEL_BIN = $(BUILD_DIR)/full_kernel.bin
OS_BIN = $(BUILD_DIR)/OS.bin

$(shell mkdir -p $(BUILD_DIR))

export PATH := $(PATH):/usr/local/i386elfgcc/bin

all: $(OS_BIN)

$(BOOT_BIN): $(BOOT_ASM)
	$(NASM) $< -f bin -o $@

$(KERNEL_ENTRY_O): $(KERNEL_ENTRY_ASM)
	$(NASM) $< -f elf -o $@

$(KERNEL_O): $(KERNEL_C)
	$(CC) -ffreestanding -m32 -g -c $< -o $@

$(PRINT_CHAR_O): print_char.c print_char.h
	$(CC) -ffreestanding -m32 -g -c $< -o $@

$(SHELL_O): simple_shell.c
	$(CC) -ffreestanding -m32 -g -c $< -o $@

$(ZEROES_BIN): $(ZEROES_ASM)
	$(NASM) $< -f bin -o $@

$(FULL_KERNEL_BIN): $(KERNEL_ENTRY_O) $(KERNEL_O) $(SHELL_O) $(PRINT_CHAR_O)
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

$(OS_BIN): $(BOOT_BIN) $(FULL_KERNEL_BIN) $(ZEROES_BIN)
	cat $^ > $@

run: $(OS_BIN)
	$(QEMU) -drive format=raw,file=$(OS_BIN),index=0,if=floppy -m 128M

clean:
	rm -rf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.o

.PHONY: all run clean

