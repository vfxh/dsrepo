#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct queue {
    int cap;     // 数组容量
    int len;     // 当前元素个数
    int rear;    // 尾下标
    int front;   // 头下标
    int* array;  // 底层数组
} Queue, *Qptr;

void isNullPtr(void* ptr) {
    if (ptr == NULL) {
        printf("ptr is NULL");
        exit(EXIT_FAILURE);
    }
}

// 入队
void enqueue(Qptr qptr) {
    isNullPtr(qptr);
    isNullPtr(qptr->array);

    // 判断
}

// 出队
void dequeue() {}

Qptr create(int count) {
    Qptr qptr = malloc(sizeof(Queue));
    isNullPtr(qptr);

    qptr->len = 0;
    qptr->cap = count;
    qptr->front = qptr->rear = 0;

    qptr->array = malloc(sizeof(int) * count);
    isNullPtr(qptr->array);

    return 0;
}

int main(void) {
    return 0;
}