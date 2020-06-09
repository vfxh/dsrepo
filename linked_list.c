/*
    单向链表实现：
        1. 不支持随机读取或更新数据
        2. 非连续存储，每个节点的地址不是相邻的
        3. 插入和删除元素不需要移动元素
        4. 长度不固定，在系统允许的前提下，长度无限长
*/
/*
    特性：
        1. 查找平均O(n)，因为是单向的，只能从头节点一个个往后查找
*/

/*
    链表和数组的适用场景：
        1. 数组适用于读取频繁，写操作少的场景，因为写操作需要移动元素
        2.
   链表对于删除和插入元素灵活，对于双向链表来说插入和删除都是O(1)的，如果不考虑查找的话
*/

/*
    1. 初始化
        head ——> NULL <—— tail

    2. 插入节点

        +-----+     +-----+     +-----+
        |  4  |---->|  1  |---->|  6  |---->NULL
        +-----+     +-----+     +-----+
           |                       |
          head                    tail
*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

struct Node;
typedef struct Node Node;
typedef struct Node* NodePtr;

// 链表节点
struct Node {
    int data;
    NodePtr next;
};

// 指向链表的数据结构
typedef struct LList {
    int len;       // 链表长度
    NodePtr head;  // 指向链表头节点
    NodePtr tail;  // 指向链表尾节点
} LList, *LListPtr;

void exitErr(const char* errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(EXIT_FAILURE);
}

void iSOutOfMemory(void* ptr) {
    if (ptr == NULL) {
        exitErr("out of memoery");
    }
}

void isInit(void* ptr) {
    if (ptr == NULL) {
        exitErr("list not init");
    }
}

void printList(LListPtr lPtr) {
    isInit(lPtr);

    for (NodePtr tmp = lPtr->head; tmp != NULL; tmp = tmp->next) {
        printf("%d\t", tmp->data);
    }

    printf("\n");
    printf("head: %d\ttail: %d\tlen: %d\n", lPtr->head->data, lPtr->tail->data,
           lPtr->len);
}

NodePtr previous(LListPtr lPtr, int pos) {
    // 判断链表是否已经初始化
    if (lPtr->head == NULL) {
        return NULL;
    }
    // 查找指定位置的前一个节点的指针
    int count = 0;
    NodePtr tmp = lPtr->head;
    while (tmp != NULL) {
        ++count;
        if (count == pos - 1) {
            break;
        }
        tmp = tmp->next;
    }

    return tmp;
}

// 插入元素
// 插入头部和插入尾部时间复杂度都是O(1)
void push(LListPtr l, int pos, int el) {
    // 检查l是否已经初始化
    isInit(l);
    // 检查位置的合法性
    // l->len + 1表示插入尾节点的下一个节点
    if (pos <= 0 || pos > l->len + 1) {
        exitErr("pos error");
    }
    // 构造新节点
    NodePtr nodePtr = malloc(sizeof(Node));
    iSOutOfMemory(nodePtr);
    memset(nodePtr, 0, sizeof(Node));
    nodePtr->data = el;
    // 插入链表头部
    if (pos == 1) {
        if (l->head == NULL) {
            // 如果当前链表为空
            l->head = l->tail = nodePtr;
        } else {
            // 如果链表不为空
            nodePtr->next = l->head;
            l->head = nodePtr;
        }
    } else if (l->len + 1 == pos) {  // 插入如尾部
        nodePtr->next = l->tail->next;
        l->tail->next = nodePtr;
        l->tail = nodePtr;
    } else {  // 插入除了头部和尾部以外的地方
        // 获取当前节点的前一个节点
        NodePtr posPreNode = previous(l, pos);
        if (posPreNode == NULL) {
            exitErr("get previous node failed");
        }
        nodePtr->next = posPreNode->next;
        posPreNode->next = nodePtr;
    }
    // 增加链表长度
    ++l->len;
}

// 删除首节点时间复杂度O(1)
// 删除其它位置的节点平均时间复杂度O(n)，因为需要遍历找到前一个节点
void pop(LListPtr l, int pos) {
    isInit(l);
    // 检查位置的合法性
    if (pos <= 0 || pos > l->len + 1) {
        exitErr("pos error");
    }

    NodePtr tmp = NULL;
    if (pos == 1) {  // 删除首节点
        tmp = l->head;
        l->head = l->head->next;
    } else {  // 删除其它位置的节点
        NodePtr preNode = previous(l, pos);
        if (preNode == NULL) {
            exitErr("get previous node failed");
        }
        // 如果是删除尾节点就需要调整tail指针
        // 删除尾节点
        if (pos == l->len) {
            l->tail = preNode;
        }
        tmp = preNode->next;
        preNode->next = tmp->next;
    }

    if (tmp != NULL) {
        printf("delete node: %d\n", tmp->data);
        free(tmp);
    } else {
        exitErr("delete node error");
    }

    --l->len;
}

LListPtr createLList() {
    LListPtr lPtr = malloc(sizeof(LList));

    iSOutOfMemory(lPtr);
    memset(lPtr, 0, sizeof(LList));
    // 初始化
    lPtr->head = lPtr->tail = NULL;
    lPtr->len = 0;

    return lPtr;
}

int main(void) {
    // 测试头部插入
    LListPtr lPtr = createLList();
    for (int i = 0; i < 3; i++) {
        push(lPtr, 1, i);
    }
    printf("测试头部插入: \n");
    printList(lPtr);

    // 测试尾部插入
    for (int i = 0; i < 3; i++) {
        push(lPtr, lPtr->len + 1, i);
    }
    printf("测试尾部插入: \n");
    printList(lPtr);

    // 测试非首尾插入节点
    push(lPtr, 1, 7);
    printf("测试插入7到第1个位置: \n");
    printList(lPtr);

    push(lPtr, 2, 8);
    printf("测试插入8到第2个位置: \n");
    printList(lPtr);

    push(lPtr, 5, 9);
    printf("测试插入9到第5个位置: \n");
    printList(lPtr);

    // 测试删除首节点
    printf("测试删除首节点：\n");
    pop(lPtr, 1);
    printList(lPtr);

    // 测试删除尾节点
    printf("测试删除尾节点：\n");
    pop(lPtr, lPtr->len);
    printList(lPtr);

    // 测试删除中间任意节点
    printf("测试删除第4个节点：\n");
    pop(lPtr, 4);
    printList(lPtr);

    return 0;
}