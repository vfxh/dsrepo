/*
    链表实现栈
        1. 没有大小限制
*/

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 节点结构体
typedef struct Node {
    int data;
    struct Node* next;
} Node, *NodePtr;

// 栈结构
typedef struct Stack {
    int len;      // 栈得元素数量
    NodePtr top;  // 指向栈顶
} Stack, *Sptr;

void exitErr(void* ptr) {
    if (ptr == NULL) {
        printf("ptr is NULL\n");
        exit(EXIT_FAILURE);
    }
}

void stackEmpty(NodePtr ptr) {
    if (ptr == NULL) {
        printf("stack is empty\n");
        exit(EXIT_FAILURE);
    }
}

void push(Sptr ptr, int el) {
    NodePtr node = malloc(sizeof(Node));
    exitErr(node);
    node->data = el;

    // 空栈
    if (ptr->top == NULL) {
        ptr->top = node;
        node->next = NULL;
    } else {  // 非空栈
        node->next = ptr->top;
        ptr->top = node;
    }

    ++ptr->len;
}

void pop(Sptr ptr) {
    exitErr(ptr);
    stackEmpty(ptr->top);

    NodePtr node = ptr->top;
    ptr->top = ptr->top->next;

    printf("el: %d pop\n", node->data);
    free(node);
    --ptr->len;
}

void top(Sptr ptr) {
    exitErr(ptr);
    stackEmpty(ptr->top);

    printf("top: %d\n", ptr->top->data);
}

Sptr createStack() {
    Sptr ptr = malloc(sizeof(Stack));
    exitErr(ptr);

    // 初始化
    ptr->len = 0;
    ptr->top = NULL;

    return ptr;
}

int main() {
    Sptr ptr = createStack();

    // 测试压栈
    for (int i = 0; i < 10; i++) {
        push(ptr, i + 1);
    }
    // 输出栈元素
    for (NodePtr tmp = ptr->top; tmp != NULL; tmp = tmp->next) {
        printf("%d\t", tmp->data);
    }
    printf("\n");
    // 测试出栈
    while (true) {
        pop(ptr);
    }
}
