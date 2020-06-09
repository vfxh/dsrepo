/*
    链式队列:

    空队列

        front ——> NULL <—— rear

    有一个节点的队列
        +-----+
        |  10 |------> NULL
        +-----+
        |     |
        front rear

    有多个节点的队列
        +-----+        +-----+        +-----+
        |  10 |------->|  6  |------->|  8  |------>NULL
        +-----+        +-----+        +-----+
           |                             |
           front                         rear
    2入队
        +-----+        +-----+        +-----+       +-----+
        |  10 |------->|  6  |------->|  8  |------>|  2  |------>NULL
        +-----+        +-----+        +-----+       +-----+
           |                                            |
           front                                        rear
    10出队
        +-----+        +-----+        +-----+       +-----+
        |  10 |        |  6  |------->|  8  |------>|  2  |------>NULL
        +-----+        +-----+        +-----+       +-----+
         |                 |                            |
         free              front                        rear
*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node, *NodePtr;

typedef struct Queue {
    int len;              // 队列当前元素个数
    NodePtr rear, front;  // 头尾指针
} Queue, *Qptr;

void isNullPtr(void* ptr) {
    if (ptr == NULL) {
        printf("null ptr\n");
        exit(EXIT_FAILURE);
    }
}

void enqueue(Qptr ptr, int el) {
    isNullPtr(ptr);

    NodePtr node = malloc(sizeof(Node));
    isNullPtr(node);
    node->data = el;

    if (ptr->front == ptr->rear && ptr->front == NULL) {  // 空队列
        node->next = NULL;
        ptr->front = ptr->rear = node;
    } else {  // 非空队列
        node->next = ptr->rear->next;
        ptr->rear->next = node;
        ptr->rear = node;
    }

    printf("el[%d] enqueue\n", el);
    ++ptr->len;
}

void dequeue(Qptr ptr) {
    isNullPtr(ptr);

    NodePtr node = NULL;
    if (ptr->front == ptr->rear) {
        // 空队列
        if (ptr->front == NULL) {
            printf("empty queue\n");
            exit(EXIT_FAILURE);
        } else {  // 只有一个节点的队列
            node = ptr->front;
            ptr->front = ptr->rear = NULL;
        }
    } else {
        node = ptr->front;
        ptr->front = ptr->front->next;
    }

    printf("el[%d] dequeue\n", node->data);
    free(node);
    --ptr->len;
}

Qptr create() {
    Qptr qptr = malloc(sizeof(Queue));
    isNullPtr(qptr);

    // 初始化
    qptr->len = 0;
    qptr->front = qptr->rear = NULL;

    return qptr;
}

int main(void) {
    Qptr qptr = create();

    // 测试入队
    for (int i = 0; i < 5; i++) {
        enqueue(qptr, i + 1);
    }

    // 测试出队
    while (qptr->len != 0) {
        dequeue(qptr);
    }

    return 0;
}