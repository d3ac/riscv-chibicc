#include <stdio.h>
#include <stdlib.h>

int main(int Argc, char *Argv[]){
    // Argv[0]表示为程序名, Argv为传入的第一个参数
    if(Argc != 2){
        fprintf(stderr, "%s: invalid number of arguments\n", Argv[0]);
        return 1; // 返回值不为0表示程序异常退出, 有错误
    }

    // 生成汇编代码
    printf("  .globl main\n");
    printf("main:\n");

    printf("  li a0, %d\n", atoi(Argv[1]));
    printf("  ret\n");
    // // 获取传入的参数, strol函数将字符串转换为长整型, 第一个参数为字符串, 第二个参数为指向字符串的指针, 第三个参数为进制
    // // 运行完了之后, P指向字符串的下一个字符 (非数字字符)
    // char *P = Argv[1];
    // printf("  li a0, %ld\n", strtol(P, &P, 10));
    
    // while(*P){
    //     if(*P == '+'){
    //         ++P; // 跳过+
    //         printf("  addi a0, a0, %ld\n", strtol(P, &P, 10));
    //         continue;
    //     }
    //     else if(*P == '-'){
    //         ++P; // 跳过-
    //         printf("  subi a0, a0, -%ld\n", strtol(P, &P, 10)); // riscv没有subi指令, 所以用subi a0, a0, -x代替
    //         continue;
    //     }
        
    // }
    return 0;
}