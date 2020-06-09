// 散列表：分离链接法实现
/*
    模型
            i  bucket
  hptr------->+-----+       +-------+
            0 | ptr |------>| value |----->NULL
              +-----+       +-------+
            1 |     |
              +-----+
            2 |     |
              +-----+       +--------+      +--------+
            3 | ptr |------>| value1 |----->| value2 |----->NULL
              +-----+       +--------+      +--------+
            4 |     |
              +-----+
            5 |     |
              +-----+
            6 |     |
              +-----+
            7 |     |
              +-----+

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

// bucket指向指针数组的指针，这个数组的每个元素的类型是指向链表的指针类型
typedef struct hash {
    int cap;
    Nodeptr* bucket;
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
        // 如果key不存在，就插入到链表
        Nodeptr newNode = malloc(sizeof(Node));
        if (newNode == NULL) {
            errExit("out of memory");
        }
        Nodeptr h = hptr->bucket[hashFunc(key, hptr)];
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
