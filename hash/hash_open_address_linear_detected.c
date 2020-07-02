#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LOAD_FACTOR_MAX 0.45

// legitimate：表示当前项已经被占用
// empty：表示当前项可用
// deleted：表示当前项已经被删除，但是为了完整性还保留着数据，可用
enum kindOfState { legitimate, empty, deleted };

typedef struct item {
    int32_t el;
    enum kindOfState state;
} Item, *Itemptr;

typedef struct hash {
    Itemptr array;  // 存储元素的数组
    float lfactor;  // 负载因子
    uint16_t cap;   // 表的容量
    uint16_t len;   // 表的当前元素个数
} Hash, *Hashptr;

void errExit(const char* errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(EXIT_FAILURE);
}

void printInfo(Hashptr hptr) {
    if (hptr == NULL) {
        return;
    }
    printf("load factor: %0.2f\tlen: %d\tcap: %d\n", hptr->lfactor, hptr->len,
           hptr->cap);
    for (int i = 0; i < hptr->cap; i++) {
        if (hptr->array[i].state == legitimate) {
            printf("%d  %d\n", i, hptr->array[i].el);
        } else {
            printf("%d  %s\n", i, 0);
        }
    }
}

// 取素数
bool isPrime(int16_t digit) {
    // 素数是指，大于1的任意自然数，除了可以被1和本身整除的数
    // 所以2是最小的素数
    if (digit <= 1) {
        return false;
    }
    int i = 2;
    for (; i < digit; i++) {
        // 可以被整除，不是素数
        if (digit % i == 0) {
            break;
        }
    }
    // 循环自然结束，说明是素数
    if (i == digit) {
        return true;
    }
    return false;
}

// 一种获取比非素数大的最小素数的垃圾算法
uint16_t upToPrime(int16_t num) {
    while (true) {
        if (!isPrime(num)) {
            ++num;
        } else {
            break;
        }
    }
    return num;
}

uint16_t hashfunc(Hashptr hptr, uint16_t key) {
    return key % hptr->cap;
}

// 如果找到就返回该key所在的散列表项的索引，找不到返回
// 返回的情况还有一个，那就是新插入的key已经存在，那么也会返回
// 该函数的调用者会判断表项的状态来决定具体怎么做
uint16_t find(Hashptr hptr, uint16_t key) {
    uint16_t pos = hashfunc(hptr, key);
    // hash_pos用于调试
    uint16_t hash_pos = pos;
    uint16_t i = 0;
    while (hptr->array[pos].state != empty && hptr->array[pos].el != key) {
        // 线性探测法
        // ++pos;
        uint16_t pre_pos = pos;
        // 平方探测法
        pos = 1 << ++i - 1;
        pos = pos % hptr->cap;
        printf("key[%d] hash pos[%d] next detected pos[%d]\n", key, hash_pos,
               pos);
        // 当i递增的次数达到表的大小时不在探测
        if (i >= hptr->cap - 1) {
            break;
        }
    }
    return pos;
}

void inithash(Hashptr hptr, uint16_t initcap) {
    hptr->lfactor = 0.0;
    hptr->len = 0;
    // 取素数
    hptr->cap = upToPrime(initcap);
    hptr->array = malloc(sizeof(Item) * hptr->cap);
    if (hptr->array == NULL) {
        errExit("out of memory");
    }
    memset(hptr->array, 0, sizeof(Item) * hptr->cap);
    // 初始化数组的每个项，都设置为empty
    for (int i = 0; i < hptr->cap; i++) {
        hptr->array[i].state = empty;
    }
}

void insertIntoArray(Hashptr hptr, uint16_t key) {
    uint16_t i = find(hptr, key);
    // 只要不是legitimate状态就可以插入这个元素
    // 即使是deleted也无关紧要，deleted存在的初衷是为了保证表的完整性
    // 可以防止删除元素之后查找失效的问题。用新的元素覆盖掉已经删除元素
    // 的数据并不影响后面的查找
    if (hptr->array[i].state != legitimate) {
        hptr->array[i].el = key;
        hptr->array[i].state = legitimate;
        // 更新负载因子和长度
        ++hptr->len;
        hptr->lfactor = (float)hptr->len / (float)hptr->cap;
    }
}

// 扩容哈希表
void growhash(Hashptr hptr) {
    if (hptr->lfactor >= LOAD_FACTOR_MAX) {
        printf("grow...\n");
        uint16_t oldcap = hptr->cap;
        // 扩容了两倍
        uint16_t doublecap = oldcap + oldcap;
        // 取出旧数组
        Itemptr oldArray = hptr->array;
        // 重新初始化哈希表
        inithash(hptr, doublecap);
        // 迁移数组元素
        for (int i = 0; i < oldcap; i++) {
            if (oldArray[i].state == legitimate) {
                insertIntoArray(hptr, oldArray[i].el);
            }
        }
        // 释放旧数组数据
        free(oldArray);
    }
}

void insert(Hashptr hptr, uint16_t key) {
    if (hptr == NULL) {
        errExit("need to init before insert");
    }
    // 忽略array的检查
    growhash(hptr);
    insertIntoArray(hptr, key);
}

void erase(Hashptr hptr, uint16_t key) {
    if (hptr == NULL) {
        errExit("need to init before erase");
    }
    // 忽略array的检查
    uint16_t i = find(hptr, key);
    if (hptr->array[i].state != legitimate) {
        printf("key[%d] does not exits\n", key);
        return;
    }
    // 懒惰删除
    // 不清空数据，只是标记这个元素呈删除状态
    hptr->array[i].state = deleted;
    // 更新当前元素个数
    --hptr->len;
    // 更新负载因子
    hptr->lfactor = (float)hptr->len / (float)hptr->cap;
    printf("deleted key: %d\n", key);
}

Hashptr init(uint16_t initcap) {
    Hashptr hptr = malloc(sizeof(Hash));
    if (hptr == NULL) {
        errExit("out of memory");
    }
    memset(hptr, 0, sizeof(Hash));
    inithash(hptr, initcap);
    return hptr;
}

int main(void) {
    int arr[9] = {47, 7, 29, 11, 9, 87, 54, 20, 30};
    Hashptr hptr = init(11);
    for (int i = 0; i < 9; i++) {
        insert(hptr, arr[i]);
    }
    printInfo(hptr);
    // erase(hptr, 47);
    // printInfo(hptr);
}

