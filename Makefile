rvcc:
	riscv64-unknown-elf-gcc -o rvcc main.c

test: rvcc # 测试rvcc
	qemu-riscv64 -L $RISCV/sysroot rvcc 33
	qemu-riscv64 -L $RISCV/sysroot rvcc 33 > tmp.s
	chmod +x *.sh
	./test.sh

clean: # 清理编译产生的文件
	rm -f rvcc *.o *.s tmp* a.out