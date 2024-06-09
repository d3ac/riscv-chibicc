riscv64-unknown-elf-gcc -static tmp.s -o tmp
qemu-riscv64 -L $RISCV/sysroot tmp
echo $?