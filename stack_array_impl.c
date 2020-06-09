/*  栈数组实现
    栈的特点：
        1. 先进后出
        2. 只有一端是开放的，压栈和出栈都在同一个入口操作
        3. 压栈和出栈都是O(1)
*/

/*
    +    +
    |    | <----- stack top
    +----+
    |  3 |
    +----+
    |  4 |
    +----+ <----- stack bottom

*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

const int zeroIndex = -1;

// 实现不包含扩容
typedef struct stack {
    int cap;    // 容量
    int len;    // 当前元素个数
    int top;    // 栈顶元素
    int *array; // 底层数组
} Stack, *Sptr;

void exitErr(const char *errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(EXIT_FAILURE);
}

void iSOutOfMemory(void *ptr) {
    if (ptr == NULL) {
        exitErr("out of memoery");
    }
}

void isInit(void *ptr) {
    if (ptr == NULL) {
        exitErr("list not init");
    }
}

// 出栈顺序
void printStack(Sptr ptr) {
    for (int i = ptr->len - 1; i >= 0 ; i--) {
        printf("%d\t", ptr->array[i]);
    }
    printf("\ncap: %d | len: %d | top index[%d]: %d\n", ptr->cap, ptr->len, ptr->top, ptr->array[ptr->top]);
}

// 压栈
void push(Sptr ptr, int el) {
    isInit(ptr);

    // 判断是否还有空间
    if (ptr->len == ptr->cap) {
        printf("el[%d] push filed!\n");
        exitErr("stack full!");
    }    

    // 增加元素
    ptr->array[++ptr->top] = el;
    ++ptr->len;
}

// 出栈
void pop(Sptr ptr) {
    isInit(ptr);

    // 判断栈是否为空
    if (ptr->len == 0) {
        exitErr("stack is empty");
    }

    // 出栈元素
    int el = ptr->array[ptr->top--];
    --ptr->len;
    
    printf("pop: %d\n", el);
}

// 取栈顶元素
int top(Sptr ptr) {
    isInit(ptr);

    // 判断底层数组是否已经初始化
    if (ptr->array == NULL) {
        exitErr("stack must init");
    }

    // 判断是否存在元素
    if (ptr->len == 0) {
        exitErr("not el");
    }

    return ptr->array[ptr->top];
}

// 创建
Sptr create(int cap) {
    // 初始化stack结构
    Sptr ptr = malloc(sizeof(Stack));
    iSOutOfMemory(ptr);
    memset(ptr, 0, sizeof(Stack));
    
    // 初始化参数
    ptr->cap = cap;
    ptr->len = 0;
    ptr->top = zeroIndex; // 下标为0指向第一个元素，没有元素指向-1

    // 初始化底层数组 
    ptr->array = malloc(sizeof(int) * ptr->cap);
    iSOutOfMemory(ptr);

    return ptr;
}

int main(void) {
    Sptr ptr = create(5);
    
    // 测试压栈
    for (int i = 0; i < 5; i++) {
        push(ptr, i+1);
    }
    printStack(ptr);
    // 测试出栈
    for (int i = 0; i < 5; i++) {
        pop(ptr);
    }
}