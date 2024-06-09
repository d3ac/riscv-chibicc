PARAMS_FILE := input.in
PARAMS := $(shell cat $(PARAMS_FILE))

rvcc:
	riscv64-unknown-elf-gcc -o rvcc main.c

test: rvcc # 测试rvcc
	qemu-riscv64 -L $RISCV/sysroot rvcc $(PARAMS)
	qemu-riscv64 -L $RISCV/sysroot rvcc $(PARAMS) > tmp.s
	chmod +x *.sh
	./test.sh

clear: # 清理编译产生的文件
	rm -f rvcc *.o *.s tmp* a.out