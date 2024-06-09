#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>

static char * CurrentInput;      

static void verrorAt(char *Loc, char *Fmt, va_list VA){
    fprintf(stderr, "%s\n", CurrentInput);
    int Pos = Loc - CurrentInput;
    fprintf(stderr, "%*s", Pos, ""); // 输出Pos个空格
    fprintf(stderr, "^ ");
    vfprintf(stderr, Fmt, VA);
    fprintf(stderr, "\n");
    va_end(VA);
    exit(1);
}

static void errorAt(char *Loc, char *Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    verrorAt(Loc, Fmt, VA);
}

typedef enum {
  TK_IDENT,   // 标记符，可以为变量名、函数名等
  TK_PUNCT,   // 操作符如： + -
  TK_KEYWORD, // 关键字
  TK_STR,     // 字符串字面量
  TK_NUM,     // 数字
  TK_PP_NUM,  // 预处理数值
  TK_EOF,     // 文件终止符，即文件的最后
} tokenKind;

typedef struct Token Token;
struct Token{
    tokenKind Kind;
    Token *Next;
    int val;
    char *Loc;
    int len;
};

static Token *newToken(tokenKind Kind, char *Start, char *End){
    Token *Tok = calloc(1, sizeof(Token)); // 分配内存空间, 1表示分配一个Token的大小, sizeof(Token)表示Token的大小
    Tok->Kind = Kind;
    Tok->Loc = Start;
    Tok->len = End - Start;
    return Tok;
}

static void errorTok(Token *Tok, char *Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    verrorAt(Tok->Loc, Fmt, VA);
}

static void error(char *Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    vfprintf(stderr, Fmt, VA);
    fprintf(stderr, "\n");
    va_end(VA);
    exit(1);
}

static Token *tokenize(){
    char *P = CurrentInput;
    Token Head  = {};
    Token *Cur = &Head;
    while(*P){
        // 跳过所有空白符
        if (isspace(*P)){
            ++P;
            continue;
        }
        
        if(isdigit(*P)){
            Cur->Next = newToken(TK_NUM, P, P);
            Cur = Cur->Next;
            const char *OldPtr = P;
            Cur->val = strtoul(P, &P, 10);
            Cur->len = P - OldPtr;
            continue;
        }

        if (*P == '+' || *P == '-'){
            Cur->Next = newToken(TK_PUNCT, P, P + 1);
            Cur = Cur->Next;
            ++P;
            continue;
        }
        errorAt(P, "invalid token: %c", *P);
    }
    Cur->Next = newToken(TK_EOF, P, P);
    return Head.Next;
}

static bool equal(Token *Tok, char *Str){
    return memcmp(Tok->Loc, Str, Tok->len) == 0 && Str[Tok->len] == '\0';
}

static Token *skip(Token * Tok, char *Str){
    if (!equal(Tok, Str))
        errorTok(Tok, "expected '%s'", Str);
    return Tok->Next;
}

static int getNumber(Token *Tok){
    if(Tok->Kind != TK_NUM)
        error("expected a number");
    return Tok->val;
}

int main(int Argc, char *Argv[]){
    // Argv[0]表示为程序名, Argv为传入的第一个参数
    if(Argc != 2){
        fprintf(stderr, "%s: invalid number of arguments\n", Argv[0]);
        return 1; // 返回值不为0表示程序异常退出, 有错误
    }
    CurrentInput = Argv[1];
    Token *Tok = tokenize();

    // 生成汇编代码
    printf("  .globl main\n");
    printf("main:\n");

    // 获取传入的参数, strol函数将字符串转换为长整型, 第一个参数为字符串, 第二个参数为指向字符串的指针, 第三个参数为进制
    // 运行完了之后, P指向字符串的下一个字符 (非数字字符)
    char *P = Argv[1];
    printf("  li a0, %d\n", getNumber(Tok));
    Tok = Tok->Next;
    
    while(Tok->Kind != TK_EOF){
        if(equal(Tok, "+")){
            Tok = Tok->Next;
            printf("  addi a0, a0, %d\n", getNumber(Tok));
            Tok = Tok->Next;
            continue;
        }
        Tok = skip(Tok, "-");
        printf("  addi a0, a0, -%d\n", getNumber(Tok));
        Tok = Tok->Next;
    }

    printf("  ret\n");
    return 0;
}