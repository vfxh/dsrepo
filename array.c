/*
    动态数组实现：
        1. 数组支持随机读取，更新数据（按索引读取或更新）
        2. 数组长度固定，顺序存储，插入和删除数据需要移动元素
        3. 增加数组超过数组长度的时候需要扩容，重新移动数据到新的数组
*/
/*
    特性：
        1. 获取和更新数据平均都是O(1)，使用下标
        3. 删除和插入平均都是O(n)，因为需要定位和迁移元素
*/
/*注意：如果失败退出不是终止程序，那么就需要释放掉新分配的内存，避免内存泄漏*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct array {
    int cap;   // 当前容量
    int len;   // 当前元素长度
    int last;  // 最后一个元素下标
    int* arr;  // 数组
} Array, *ArrayPtr;

// 输出错误并终止程序
void exitErr(const char* errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(EXIT_FAILURE);
}

// 检查malloc返回
void isOutOfMemmory(void* ptr) {
    if (ptr == NULL) {
        exitErr("out of memory");
    }
}

// 未初始化
void checkPtr(ArrayPtr ptr) {
    if (ptr == NULL) {
        exitErr("not init array");
    }
}

// 增加元素，元素可能会移动
// 如果pos小于0则添加到头部，如果超过数据长度，则会添加失败
// 插入第一个位置下标为0，插入第n个位置，下标为n-1
void insert(ArrayPtr ptr, int pos, int el) {
    checkPtr(ptr);

    if (pos > ptr->len + 1) {
        exitErr("pos error");
    }
    // 如果数组容量已经满了就扩容
    if (ptr->cap == ptr->len) {
        // 分配基于当前容量的两倍内存
        int* newArr = malloc(sizeof(int) * (ptr->cap * 2));
        isOutOfMemmory(newArr);
        memset(newArr, 0, sizeof(int) * (ptr->cap * 2));
        // 迁移数据
        // void* memcpy(void *dst, const void *src, size_t size);
        // memcpy将src的前size个字节拷贝到dst中
        newArr = memcpy(newArr, ptr->arr, sizeof(int) * ptr->len);
        isOutOfMemmory(newArr);
        // 释放掉原来的内存
        free(ptr->arr);
        ptr->arr = NULL;
        // 指向新分配的数组
        ptr->arr = newArr;
        ptr->cap *= 2;
    }

    // 如果位置小于0就默认插入第一个位置s
    if (pos < 0) {
        pos = 1;
    }
    // 迁移元素
    for (int i = ptr->len; i > pos - 1; i--) {
        ptr->arr[i] = ptr->arr[i - 1];
    }
    // 赋值
    ptr->arr[pos - 1] = el;
    ++ptr->last;
    ++ptr->len;
}

// 获取元素

// 更新元素

// 删除元素，元素会移动
// i 是要删除的元素索引
void erase(ArrayPtr ptr, int i) {
    checkPtr(ptr);
    // 不存在元素就直接结束，不做任何操作
    if (ptr->len == 0) {
        return;
    }
    if (i >= ptr->len || i < 0) {
        exitErr("out of range");
    }
    int el = ptr->arr[i];
    if (i < ptr->last) {
        while (i <= ptr->last) {
            ptr->arr[i] = ptr->arr[i + 1];
            ++i;
        }
    }
    // 移动last重新指向最后一个元素
    --ptr->last;
    // 减少元素个数
    --ptr->len;
    printf("remove el: %d\n", el);
}

// 输出数组
void printInfo(ArrayPtr ptr) {
    checkPtr(ptr);
    // printf("cap: %d\nlen: %d\nlast: %d\n", ptr->cap, ptr->len, ptr->last);
    // return;
    for (int i = 0; i <= ptr->last; i++) {
        printf("%d ", ptr->arr[i]);
    }
    printf("\n");
}

// 创建
ArrayPtr createArray(int length) {
    // 创建指向结构体的指针
    ArrayPtr ptr = malloc(sizeof(Array));
    isOutOfMemmory(ptr);
    // 创建指定长度的数组
    ptr->arr = malloc(length * sizeof(int));
    isOutOfMemmory(ptr);
    // 初始化结构体属性
    memset(ptr->arr, 0, sizeof(int) * length);
    ptr->cap = length;
    ptr->len = 0;
    // last表示最后一个元素的索引
    // 因为所以从0开始，所以last初始化未-1
    ptr->last = -1;

    return ptr;
}

// 不能获取数组元素的地址，如果发生扩容，原来的元素地址就失效了
int main(void) {
    ArrayPtr arrPtr = createArray(2);
    // 测试从头部插入元素
    for (int i = 0; i < 10; i++) {
        insert(arrPtr, 1, i);
    }
    printInfo(arrPtr);
    // 测试从尾部插入元素
    insert(arrPtr, arrPtr->len + 1, 44);
    printInfo(arrPtr);
    // 测试从中间任意位置插入元素
    insert(arrPtr, 4, 1234);
    printInfo(arrPtr);
    // 测试从头部插入
    insert(arrPtr, 1, 3232);
    printInfo(arrPtr);
    // int len = arrPtr->len;
    // 测试从头部删除全部全部元素
    // for (int i = 0; i < len; i++) {
    //     erase(arrPtr, 0);
    // }
    // printInfo(arrPtr);
    // 测试从尾部删除全部元素
    // for (int i = 0; i < len; i++) {
    //     erase(arrPtr, arrPtr->len - 1);
    // }
    // printInfo(arrPtr);
    // 测试从除头尾之外的位置删除元素
    // erase(arrPtr, 3);
    // printInfo(arrPtr);
    // erase(arrPtr, 6);
    // printInfo(arrPtr);
    return 0;
}