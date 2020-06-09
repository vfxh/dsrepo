// 分离链接法实现哈希表

#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LOADFACTOR 0.65  // 出发扩容的最大负载因子
#define MAXHASHSIZE 128  // 哈希表最大长度

// 链表结点
typedef struct node {
    struct node* next;
    const char* key;    // hash元素的键
    const char* value;  // hash元素的值
} Node, *Nodeptr;

// 哈希表数据单元
typedef struct hashEl {
    Nodeptr h;    // 链表的头结点，定位一个指针
    uint8_t len;  // 当前链表的结点长度，（为了测试用的，看看链表有多少个节点）
} HashEl, *HashElptr;

// 哈希表结构
typedef struct hash {
    HashElptr* array;             // 哈希表元素
    float lfactor;                // 负载因子
    uint8_t cap;                  // 当前哈希表容量
    uint8_t len;                  // 当前哈希表长度
    uint8_t helper[MAXHASHSIZE];  // 辅助数组，用于计算负载因子
} Hash, *Hashptr;

void errExit(const char* errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(EXIT_FAILURE);
}

void isNull(Hashptr hptr, const char* key) {
    // 判断hash结构是否已经初始化
    if (hptr == NULL) {
        errExit("hptr is NULL");
    }

    // 判断键是否为空
    if (key == NULL) {
        errExit("key if NULL");
    }
}

void printInfo(Hashptr hptr) {
    if (hptr == NULL) {
        errExit("hptr is NULL");
    }

    printf(
        "----------------------------------------------------------------------"
        "----------------------------\n");
    printf("cap: %d | len: %d load-factor: %f\n", hptr->cap, hptr->len,
           hptr->lfactor);
    printf("table item: \n");
    for (int i = 0; i < hptr->cap; i++) {
        printf("%-10d\t", i);
    }
    printf("\n");
    printf("next: \n");
    for (int i = 0; i < hptr->cap; i++) {
        printf("x0%p\t", hptr->array[i]->h->next);
    }
    printf("\n");
}

// 哈希函数
uint8_t hashFunc(const char* key, Hashptr hptr) {
    isNull(hptr, key);

    uint32_t len = 0;
    while (*key++ != '\0') {
        ++len;
    }
    // 得到的返回值最大不超过hptr->cap - 1
    return len % hptr->cap;
}

// 查找结点
Nodeptr findNode(Hashptr hptr, const char* key) {
    Nodeptr tmp = NULL;
    int i = hashFunc(key, hptr);

    tmp = hptr->array[i]->h->next;
    while (tmp != NULL && (strcmp(tmp->key, key) != 0)) {
        tmp = tmp->next;
    }

    return tmp;
}

// 将元素结点添加到对应的链表头部当中
void addNodeToList(Nodeptr h, const char* key, const char* value) {
    // 构建新节点
    Nodeptr newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        errExit("out of memory");
    }
    newNode->key = key;
    newNode->value = value;

    // 插入链表头部
    newNode->next = h->next;
    h->next = newNode;
}

// 简单扩容两倍
// 扩容之后需要重新哈希分布，然后拷贝数据到新的哈希表上，释放掉原来的数据
// 并且重新计算负载因子，如果负载因子还是比原来的大，那么加入随机数，再次
// 重复以上步骤
Hashptr growHash(Hashptr hptr) {
    if (hptr->lfactor >= LOADFACTOR) {
    }

    return hptr;
}

void insert(Hashptr hptr, const char* key, const char* value) {
    if (hptr == NULL) {
        errExit("hptr is null");
    }

    if (key == NULL || value == NULL) {
        errExit("key or value is null");
    }

    Nodeptr tmp = findNode(hptr, key);
    if (tmp == NULL) {  // 如果key不存在，那么就插入键值对
        // 首先需要处理是否需要扩容

        uint8_t i = hashFunc(key, hptr);
        Nodeptr head = hptr->array[i]->h;
        addNodeToList(head, key, value);
        // 如果需要，就更新哈希表的长度和负载因子
        // 如果从辅助数组返回的结果为假，那么说明当前哈希表项没有
        // 指向一条链表，那么就需要更新哈希表长度和负载因子
        if (!hptr->helper[i]) {
            // 更新哈希表长度
            ++hptr->len;
            // 更新负载因子的值
            hptr->lfactor = hptr->len / hptr->cap;
        }
        // 更新链表长度
        ++hptr->array[i]->len;
    } else {  // 如果键已经存在，那么就更新键值
        // 默认传进来的是字符串字面量，分配在静态储存区的数组
        tmp->value = value;
    }
}

// 删除键值对
void erase(Hashptr hptr, const char* key) {
    isNull(hptr, key);

    // 找到就删除该节点，反之什么都不做
    Nodeptr tmp = hptr->array[hashFunc(key, hptr)]->h;
    if (tmp != NULL) {
        // 查找当前给定的key所属的节点的前一个结点
        while (tmp->next != NULL) {
            if ((tmp->next->key != NULL) && !strcmp(key, tmp->next->key)) {
                break;
            }
            tmp = tmp->next;
        }
        // 找到就删除节点
        if (tmp->next != NULL) {
            Nodeptr node = tmp->next;
            tmp->next = node->next;
            printf("delete key: %s\n", node->key);
            free(node);
        }
    }
}

// 初始化哈希表
Hashptr initHashTable(int inicap) {
    // 创建哈希结构
    Hashptr hptr = malloc(sizeof(Hash));
    if (hptr == NULL) {
        errExit("out of memory");
    }

    // 创建哈希表
    hptr->array = malloc(sizeof(HashEl) * inicap);
    if (hptr->array == NULL) {
        errExit("out of memory");
    }

    // 初始化各项参数
    hptr->lfactor = 0.0;
    hptr->len = 0;
    hptr->cap = inicap;
    // hptr->helper = malloc(sizeof(uint8_t) * MAXHASHSIZE);
    memset(hptr->helper, 0, sizeof(uint8_t));

    // 初始化哈希表项
    for (int i = 0; i < hptr->cap; i++) {
        hptr->array[i] = malloc(sizeof(HashEl));
        if (hptr->array[i] == NULL) {
            errExit("out of memory");
        }
        hptr->array[i]->len = 0;
        memset(&(hptr->array[i]->h), 0, sizeof(Node));
        hptr->array[i]->h->next = NULL;
        hptr->array[i]->h->key = NULL;
        hptr->array[i]->h->value = NULL;
    }

    return hptr;
}

int main(void) {
    Hashptr hptr = initHashTable(5);
    printf("the status of init: \n");
    printInfo(hptr);
    return 0;
}