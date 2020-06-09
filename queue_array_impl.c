/*
    队列数组实现
        1. 先进先出
        2. 循环队列，当出队留出空位之后就移动尾部指针指向空位
*/

/*
没有扩容机制
    循环队列：
    从rear端入队，front出队
    空队

       rear
       |
    +-----+-----+-----+-----+-----+-----+
    |     |     |     |     |     |     |
    +-----+-----+-----+-----+-----+-----+
       |
       front

    1入队
    +-----+-----+-----+-----+-----+-----+
    |  1  |     |     |     |     |     |
    +-----+-----+-----+-----+-----+-----+
       |     |
       front  rear

    5入队
    队列满：(rear+1) % len(array) == front
    +-----+-----+-----+-----+-----+-----+
    |  1  |  2  |  3  |  4  |  5  |     |
    +-----+-----+-----+-----+-----+-----+
       |                             |
       front                          rear

    1出队
    空出一个位置
    +-----+-----+-----+-----+-----+-----+
    |     |  2  |  3  |  4  |  5  |     |
    +-----+-----+-----+-----+-----+-----+
             |                       |
             front                    rear
    2出队
    空出两个位置
    +-----+-----+-----+-----+-----+-----+
    |     |     |  3  |  4  |  5  |     |
    +-----+-----+-----+-----+-----+-----+
                   |                 |
                   front              rear
    6入队
    rear指向留出来的空位
    +-----+-----+-----+-----+-----+-----+
    |     |     |  3  |  4  |  5  |  6  |
    +-----+-----+-----+-----+-----+-----+
       |           |
       rear        front

    为什么队列满的时候需要留出一个空位:
        队列满：(rear+1) % len(array) == front
    +-----+-----+-----+-----+-----+-----+
    |  7  |     |  3  |  4  |  5  |  6  |
    +-----+-----+-----+-----+-----+-----+
             |     |
             rear  front

    原因是：如果rear和front重合，那么就无法判断出当前的位置到底是队头还是队尾了
*/

/*
    求模运算：
        操作都不为正整数的情况下：
        有：x == m % n 则：0 <= x < n, x = m - nc 其中c为某个常数
        比如：1 % 6 = 1, 2 % 6 = 2 ... 6 % 6 = 0, 7 % 6 = 1 ... 以此循环
*/

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

void printfQueue(Qptr qptr) {
    for (int i = qptr->front; i != qptr->rear; i = (i + 1) % qptr->cap) {
        printf("[%d]%d\t", i, qptr->array[i]);
    }

    printf("\ncap: %d | len: %d | rear index[%d] | front index[%d]\n",
           qptr->cap, qptr->len, qptr->rear, qptr->front);
}

// 入队
void push(Qptr qptr, int el) {
    isNullPtr(qptr);
    isNullPtr(qptr->array);

    // 判断队列是否已满
    if ((qptr->rear + 1) % qptr->cap == qptr->front) {
        printf("queue full!!\n");
        exit(EXIT_FAILURE);
    }
    // 入队
    qptr->array[qptr->rear] = el;
    qptr->rear = (qptr->rear + 1) % qptr->cap;
    ++qptr->len;
}

// 出队
void pop(Qptr qptr) {
    isNullPtr(qptr);
    isNullPtr(qptr->array);

    // 判断队列是否为空
    if (qptr->rear == qptr->front) {
        printf("empty queue\n");
        return;
    } else {
        printf("el[%d] dequeue\n", qptr->array[qptr->front]);
        qptr->front = (qptr->front + 1) % qptr->cap;
        --qptr->len;
    }
}

Qptr createQueue(int count) {
    Qptr q = malloc(sizeof(Queue));
    isNullPtr(q);
    memset(q, 0, sizeof(Queue));

    if (count <= 0) {
        count = 10;
    }

    // 初始化底层数组
    q->array = malloc(sizeof(int) * count);
    isNullPtr(q->array);

    // 初始化各项参数
    q->cap = count;
    q->len = 0;
    q->front = q->rear = 0;

    return q;
}

int main(void) {
    // 创建10个元素的数组，只有9个可用
    Qptr qptr = createQueue(10);

    // 测试入队
    printf("test enqueue\n");
    for (int i = 1; i < 10; i++) {
        push(qptr, i);
    }
    printfQueue(qptr);
    // // 测试出队
    // printf("test dequeue\n");
    // for (int i = 0; i < 10 - 1; i++) {
    //     pop(qptr);
    // }
    // printfQueue(qptr);

    // 测试出队5个
    for (int i = 0; i < 10; i++) {
        pop(qptr);
    }
    printfQueue(qptr);
    // 入队一个
    printf("100 enqueue\n");
    push(qptr, 100);
    printfQueue(qptr);
    printf("11 enqueue\n");
    push(qptr, 11);
    printfQueue(qptr);
    return 0;
}