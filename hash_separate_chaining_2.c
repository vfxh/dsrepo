/* what`s the fuck hash table？
 哈希表
    哈希表也叫散列表，是根据给定的键计算出存储值位置的一种存储键值对的数据结构。
    它根据键来查找值得时间复杂度为常数级别，接近于O(1)。在理想状态下，无论存储
    得键值对有多少，它查找得时间复杂度都是O(1)。不过这是不可能的，只能趋近。

 散列函数
    哈希函数就是根据键来计算出值存储位置的代码块。一种理想的状态是：每个不同的
    键都映射到不同的位置。不过这也是不可能的，因为关键字是无限的。当关键字变得
    很多的时候，同一个位置就会存储很多不同的键对应的值。这对后面的读写操作影响
    还是不小的
 哈希冲突

 负载因子
    定义：α = 元素个数 / 散列表容量
    len是装填到数组的元素个数，cap是数组的当前容量，α是负载因子。
    负载因子和len成正比，负载因子越大，装填的元素个数越多，冲突的几率就越大
    指的是数组的元素个数和散列表容量的比值
    （所以，为了能够让负载因子更有意义。哈希函数就要设计得接近于理想化状态，
    让散列表分布均匀，元素的链表不至于太长。如果散列表分布不均匀，有些链表
    长度很长，有些很短，查找效率会变低。甚至数组元素的链表为空，那么负载因
    就失去了意义子）
 扩容
    当负载因子到达一定程度的时候，需要扩容，否则性能就会有所下降。新分配一个
    新的数组，然后重新哈希。使得数组变得稀疏，链表也相对较短，数组增加。
*/

// 分离链接法实现

/* 还需要加上负载因子
    模型
              |--------cap----|
              |----len----|   |
              +---+---+---+---+
    +-------->| 0 | 1 | 2 |...|
    |         +---+---+---+---+
    |           0   1   2   ...
    |
    |           i    bucket
  hptr------->+-----+-----+    +---+---+----+
            0 |  1  | h---|--->| k | v | n--|--->NULL
              +-----+-----+    +---+---+----+
            1 | len | h---|--->NULL
              +-----+-----+
            2 | len | h---|--->NULL
              +-----+-----+    +---+---+----+   +---+---+----+
            3 | len | h---|--->| k | v | n--|-->| k | v | n--|-->NULL
              +-----+-----+    +---+---+----+   +---+---+----+
            4 | len | h---|--->NULL
              +-----+-----+
            5 | len | h---|--->NULL
              +-----+-----+
            6 | len | h---|--->NULL
              +-----+-----+
*/

/*
    辅助数组
        其作用是统计散列表的元素数量，来计算负载因子。
        辅助数组的数量是散列表的最大数量，根据散列函数
        转换得到的散列表下标来确定当前元素是否指向一个链表
*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 链表节点
typedef struct Node {
    const char* key;
    const char* value;
    struct Node* next;
} Node, *Nodeptr;

// 数组元素
// 增加这个结构体的意义是方便查看每个数组元素对应的链表有多少个结点
typedef struct arrayEl {
    int len;    // 链表的个数
    Nodeptr h;  // 指向链表的头指针
} ListHeader, *LHptr;

// 两个字节，最大65535
typedef unsigned short u_int16_t;
// 辅助结构，辅助数组不扩容
typedef struct hashHelper {
    u_int16_t* array;  // 指向辅助数组的指针
    u_int16_t hashLen;  // 当前哈希表的长度，可以根据这个来算出负载因子
    u_int16_t cap;  // 辅助数组最大容量
} helper, *helperPtr;

// bucket指向指针数组的指针，这个数组的每个元素的类型是指向链表的指针类型
typedef struct hash {
    int cap;                // 哈希表容量
    float lfactor;          // 负载因子
    Nodeptr* bucket;        // 指向哈希表的指针
    helperPtr helperArray;  // 辅助数组
} Hash, *Hptr;

void errExit(const char* errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(EXIT_FAILURE);
}

void isNull(Hptr hptr, const char* key) {
    // 判断hash结构是否已经初始化
    if (hptr == NULL) {
        errExit("hptr is NULL");
    }

    // 判断键是否为空
    if (key == NULL) {
        errExit("key if NULL");
    }
}

void printList(Nodeptr nptr) {
    for (Nodeptr tmp = nptr->next; tmp != NULL; tmp = tmp->next) {
        printf("key: %s | value: %s\n", tmp->key, tmp->value);
    }
}

void printArray(Hptr hptr) {
    printf(
        "head: "
        "----------------------------------------------------------------------"
        "--"
        "----------\n");
    for (int i = 0; i < hptr->cap; i++) {
        printf("%-10d\t", i);
    }
    printf("\n");
    for (int i = 0; i < hptr->cap; i++) {
        printf("0x%p\t", hptr[i]);
    }
    printf(
        "\nnext: "
        "----------------------------------------------------------------------"
        "------------\n");
    for (int i = 0; i < hptr->cap; i++) {
        printf("0x%p\t", hptr->bucket[i]->next);
    }
    printf(
        "\n--------------------------------------------------------------------"
        "-------------------"
        "--\n");
}

// 扩容
// 需要重新散列并且清空辅助数组数据和重新计算
void expanstion() {}

// 散列函数
// 散列函数的作用是把键转化为数组指针的下标
// 不能直接用strlen函数或sizeof函数计算key的长度，因为参数是一个指针
int hashFunc(const char* key, Hptr hptr) {
    unsigned int len = 0;

    isNull(hptr, key);

    // 获取key的字符串长度
    while (*key++ != '\0') {
        ++len;
    }

    return len % hptr->cap;
}

// 查找链表节点
// 找到值和key相同的节点就返回当前节点的指针，并在插入函数更新它的值
// 如果找不到就证明不存在，返回NULL，插入函数会在链表的头部插入新的节点
Nodeptr find(Hptr hptr, const char* key) {
    Nodeptr tmp = NULL;
    // 获取key转化的数组下标
    int i = hashFunc(key, hptr);

    // 循环查找对应的链表
    tmp = hptr->bucket[i]->next;
    while (tmp != NULL && (strcmp(tmp->key, key) != 0)) {
        tmp = tmp->next;
    }

    return tmp;
}

// 插入键值对
// 当前实现是插入键值对，键是用来计算值所在链表所对应的数组下标的
// 遍历链表确定key是否存在，如果存在就更新它的值，如果不存在就在链表头部插入key/value
void insert(Hptr hptr, const char* key, const char* value) {
    if (key == NULL || value == NULL) {
        errExit("key or value is NULL");
    }

    if (hptr == NULL) {
        errExit("hptr is NULL");
    }

    Nodeptr tmp = find(hptr, key);
    if (tmp == NULL) {
        // 获取哈希表数据单元下标
        size_t i = hashFunc(key, hptr);

        // 处理辅助数组逻辑
        if (hptr->helperArray == NULL || hptr->helperArray->array == NULL) {
            errExit("helper or helper array is null");
        }
        // 如果当前辅助数组对应的下标已经使用，那么就更新哈希表长度
        if (!hptr->helperArray->array[i]) {
            ++hptr->helperArray->hashLen;
            // 判断负载因子
            if (hptr->helperArray->hashLen / hptr->cap >= 0.75) {
                // 扩容
            }
        }

        // 如果key不存在，就插入到链表
        Nodeptr newNode = malloc(sizeof(Node));
        if (newNode == NULL) {
            errExit("out of memory");
        }
        Nodeptr h = hptr->bucket[i];
        newNode->next = h->next;
        h->next = newNode;
        newNode->key = key;
        newNode->value = value;
    } else {
        // 如果key已经存在，那么就更新它的值
        tmp->value = value;
    }
}

// 删除键值对
void erase(Hptr hptr, const char* key) {
    isNull(hptr, key);

    // 找得到就删除，反之什么都不做
    // 需要处理
    // 这个查找前一个节点的逻辑可以和find函数合并，这里暂且不处理
    Nodeptr tmp = hptr->bucket[hashFunc(key, hptr)];
    if (tmp == NULL) {
        // 结束，什么都不做
        return;
    } else {
        // 查找前一个结点
        while (tmp->next != NULL) {
            if (tmp->next->key != NULL && !strcmp(key, tmp->next->key)) {
                break;
            }
            tmp = tmp->next;
        }
        // 找到就删除结点
        if (tmp->next != NULL) {
            Nodeptr node = tmp->next;
            tmp->next = node->next;
            printf("delete key: %s\n", node->key);
            free(node);
        }
    }
}

// 获取键值对
Nodeptr get(Hptr hptr, const char* key) {
    Nodeptr tmp = hptr->bucket[hashFunc(key, hptr)];

    return find(hptr, key);
}

// 初始化hash表，count为指针数组的初始容量
Hptr create(int count) {
    // 初始化指向hash结构，返回一个指针
    Hptr hptr = malloc(sizeof(Hash));
    if (hptr == NULL) {
        errExit("out of memoery");
    }

    // 初始化容量
    hptr->cap = count;
    // 负载因子初始化为0.0
    hptr->lfactor = 0.0;

    // 辅助数组初始化
    hptr->helperArray = malloc(sizeof(helper));
    if (hptr->helperArray == NULL) {
        errExit("out of memory");
    }
    // 初始化辅助数组
    hptr->helperArray->array = malloc(sizeof(u_int16_t) * sizeof(u_int16_t));
    memset(hptr->helperArray->array, 0, sizeof(u_int16_t) * sizeof(u_int16_t));
    // 暂时定辅助数组长度为65535
    hptr->helperArray->cap = sizeof(u_int16_t);
    // 初始化时哈希表长度为0
    hptr->helperArray->hashLen = 0;

    // 给指针数组分配空间
    hptr->bucket = malloc(sizeof(Node) * count);
    if (hptr == NULL) {
        errExit("out of memoery");
    }

    // 为数组的每个元素分配空间，元素是指向链表的指针
    for (int i = 0; i < hptr->cap; i++) {
        hptr->bucket[i] = malloc(sizeof(Node));
        if (hptr == NULL) {
            errExit("out of memoery");
        }
        memset(hptr->bucket[i], 0, sizeof(Node));
        // 指针数组的元素是链表的头节点，当前实现只用于定位链表
        // 所以key和value不使用，不过还是把它们都初始化为NULL
        hptr->bucket[i]->key = NULL;
        hptr->bucket[i]->value = NULL;
        // 初始化时候没有指向的链表，所以next指向NULL
        hptr->bucket[i]->next = NULL;
    }

    return hptr;
}

int main(void) {
    Hptr hptr = create(5);
    // 打印指针数组
    printf("the status of init: \n");
    printArray(hptr);
    // 测试插入键值对
    const char* key = "name";
    const char* value = "tom";
    insert(hptr, key, value);
    printf("the status after insert: \n");
    printArray(hptr);
    // 查找
    Nodeptr nptr = get(hptr, key);
    if (nptr == NULL) {
        printf("cannot find the key: `%s`\n", key);
    } else {
        printf("key: %s | value: %s\n", nptr->key, nptr->value);
    }
    erase(hptr, key);
    Nodeptr eptr = get(hptr, key);
    if (eptr == NULL) {
        printf("key `%s` not exists\n", key);
    }

    return 0;
}
