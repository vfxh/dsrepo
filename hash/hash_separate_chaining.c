// 分离链接法实现哈希表

#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LOADFACTOR 0.75  // 出发扩容的最大负载因子
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
    HashElptr* array;             // 哈希表数组
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
        printf("0x%p\t", hptr->array[i]->h->next);
    }
    printf("\n");
}

void printKeyValue(Hashptr hptr) {
    if (hptr == NULL) {
        errExit("hptr is NULL");
    }

    for (int i = 0; i < hptr->cap; i++) {
        HashElptr item = hptr->array[i];
        printf("index: %d len: %d  ", i, item->len);
        for (Nodeptr tmp = item->h->next; tmp != NULL; tmp = tmp->next) {
            printf("`%s|%s` ", tmp->key, tmp->value);
        }
        printf("\n");
    }
}

// 哈希函数
uint8_t hashFunc(const char* key, Hashptr hptr) {
    isNull(hptr, key);

    uint32_t len = 0;
    while (*key++ != '\0') {
        //++len;
        len = (len << 5) + *key;
    }
    // 得到的返回值最大不超过hptr->cap - 1
    return len % hptr->cap;
}

void initTableItem(Hashptr hptr, uint8_t initcap) {
    // 创建哈希表
    hptr->array = malloc(sizeof(HashEl) * initcap);
    if (hptr->array == NULL) {
        errExit("out of memory");
    }

    // 初始化各项参数
    hptr->lfactor = 0.0;
    hptr->len = 0;
    hptr->cap = initcap;
    // hptr->helper = malloc(sizeof(uint8_t) * MAXHASHSIZE);
    memset(hptr->helper, 0, sizeof(uint8_t) * MAXHASHSIZE);

    // 初始化哈希表项
    for (int i = 0; i < hptr->cap; i++) {
        hptr->array[i] = malloc(sizeof(HashEl));
        if (hptr->array[i] == NULL) {
            errExit("out of memory");
        }
        hptr->array[i]->len = 0;
        hptr->array[i]->h = malloc(sizeof(Node));
        if (hptr->array[i]->h == NULL) {
            errExit("out of memory");
        }
        memset(hptr->array[i]->h, 0, sizeof(Node));
        hptr->array[i]->h->next = NULL;
        hptr->array[i]->h->key = NULL;
        hptr->array[i]->h->value = NULL;
    }
}

// 初始化哈希表
Hashptr initHashTable(int initcap) {
    // 创建哈希结构
    Hashptr hptr = malloc(sizeof(Hash));
    if (hptr == NULL) {
        errExit("out of memory");
    }
    // 初始化各项参数
    initTableItem(hptr, initcap);

    return hptr;
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
void addNodeToList(Hashptr hptr, const char* key, const char* value) {
    uint8_t i = hashFunc(key, hptr);
    Nodeptr head = hptr->array[i]->h;

    // 构建新节点
    Nodeptr newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        errExit("out of memory");
    }
    newNode->key = key;
    newNode->value = value;
    // 插入链表头部
    newNode->next = head->next;
    head->next = newNode;
    // 如果有需要就更新负载因子
    if (!hptr->helper[i]) {
        // 更新哈希表长度
        ++hptr->len;
        // 更新负载因子的值
        // 强制转成浮点类型，否则无法计算出浮点数
        hptr->lfactor = (float)hptr->len / (float)hptr->cap;
        hptr->helper[i] = 1;
        // printf("%f / %f = %f\n", (float)hptr->len,
        // (float)hptr->cap,hptr->lfactor);
    }
    // 更新链表长度
    ++hptr->array[i]->len;
}

// 简单扩容两倍
// 扩容之后需要重新哈希分布，然后拷贝数据到新的哈希表上，释放掉原来的数据
// 并且重新计算负载因子，如果负载因子还是比原来的大，那么加入随机数，再次
// 重复以上步骤（重新计算还是先不搞了，it`s too troublesome）
Hashptr growHash(Hashptr hptr) {
    if (hptr->lfactor >= LOADFACTOR) {
        uint8_t oldcap = hptr->cap;
        HashElptr* old = hptr->array;
        // 扩容两倍
        uint8_t doublecap = hptr->cap + hptr->cap;
        // 重新初始化各项参数
        initTableItem(hptr, doublecap);
        // 插入数据后自动哈希，计算负载因子
        for (int i = 0; i < oldcap; i++) {
            Nodeptr tmp = old[i]->h->next;
            // 释放掉旧的头节点
            free(old[i]->h);
            while (tmp != NULL) {
                // 插入旧数据
                addNodeToList(hptr, tmp->key, tmp->value);
                // 移动指针到下一个
                Nodeptr oldNode = tmp;
                tmp = tmp->next;
                // 释放旧节点
                free(oldNode);
            }
        }
        // 释放掉旧表数据
        free(old);
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
    printf("insert %s|%s\n", key, value);
    Nodeptr tmp = findNode(hptr, key);
    // 如果key不存在，那么就插入键值对
    if (tmp == NULL) {
        // 首先需要处理是否需要扩容
        growHash(hptr);
        // 插入新的键值对
        addNodeToList(hptr, key, value);
    } else {  // 如果键已经存在，那么就更新键值
        // 默认传进来的是字符串字面量，分配在静态储存区的数组
        tmp->value = value;
    }
}

// 删除键值对
void erase(Hashptr hptr, const char* key) {
    isNull(hptr, key);

    // 找到就删除该节点，反之什么都不做
    HashElptr item = hptr->array[hashFunc(key, hptr)];
    Nodeptr tmp = item->h;
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
            if (item->len != 0) {
                // 递减链表的节点数量
                --item->len;
            } else {
                errExit("the length of linked list is 0");
            }
            free(node);
        }
    }
}

int main(void) {
    // 初始化
    Hashptr hptr = initHashTable(5);
    printf("the status of init: \n");
    printInfo(hptr);

    // 插入键值对
    printf("insert key-value: \n");
    insert(hptr, "key-1", "value-1");
    printf("load factor: %f\n", hptr->lfactor);
    insert(hptr, "key-2", "value-2");
    printf("load factor: %f\n", hptr->lfactor);
    insert(hptr, "key-3", "value-3");
    printf("load factor: %f\n", hptr->lfactor);
    insert(hptr, "key-4", "value-4");
    printf("load factor: %f\n", hptr->lfactor);
    insert(hptr, "key-5", "value-5");
    printf("load factor: %f\n", hptr->lfactor);
    insert(hptr, "key-6", "value-6");
    printf("load factor: %f\n", hptr->lfactor);
    insert(hptr, "hello", "world");
    printf("load factor: %f\n", hptr->lfactor);
    printInfo(hptr);
    // 查找键值对
    // Nodeptr nodeptr = findNode(hptr, "key-2");
    // if (nodeptr == NULL) {
    //     printf("key: `key-2` not exists\n");
    //     exit(EXIT_FAILURE);
    // }
    printKeyValue(hptr);
    return 0;

    erase(hptr, "key-1");
    printKeyValue(hptr);
    erase(hptr, "key-2");
    printKeyValue(hptr);
    erase(hptr, "key-3");
    printKeyValue(hptr);
    erase(hptr, "key-4");
    printKeyValue(hptr);
    erase(hptr, "key-5");
    printKeyValue(hptr);
    erase(hptr, "key-6");
    printKeyValue(hptr);
    erase(hptr, "hello");
    erase(hptr, "helloni");
    printKeyValue(hptr);

    return 0;
}