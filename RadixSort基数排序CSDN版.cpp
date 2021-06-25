#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define RADIXCOUNT 10  //桶的个数，桶号：0 1 2 ..... 9
#define RANDMAX 100000 //随机数的最大值加1+

struct Node {
    int value;
    struct Node* next;
};

struct Queue {
    struct Node* head;
    struct Node* tail;
};

void getRandArray(int array[], int size);
void radixSort(int array[], int size);
void printArray(int array[], int size);
int getMaxLength(int array[], int size);
void distributeNumbers(int array[], int size, struct Queue bucket[], int dividend);
void rearrangeArray(int array[], int size, struct Queue bucket[]);
void isSorted(int array[], int size);

int main(int argc, char const* argv[])
{
    int size = 0;
    scanf_s("%d", &size);
    assert(size > 0);

    int* array = (int*)calloc(size, sizeof(int));
    getRandArray(array, size);
    printArray(array, size);

    radixSort(array, size);
    printArray(array, size);
    isSorted(array, size);

    free(array);
    return 0;
}

//利用伪随机数填充数组array
void getRandArray(int array[], int size)
{
    assert(array != NULL && size > 0);

    srand((unsigned)time(NULL));
    int i = 0;
    for (i = 0; i < size; ++i) {
        //产生RANDMAX以内的伪随机数
        array[i] = rand() % RANDMAX;
    }
}

//基数排序，按从小到大的顺序进行排列
void radixSort(int array[], int size)
{
    assert(array != NULL && size > 0);

    struct Queue bucket[RADIXCOUNT];
    int i = 0;
    for (i = 0; i < RADIXCOUNT; ++i) {
        bucket[i].head = NULL;
        bucket[i].tail = NULL;
    }

    int maxLength = getMaxLength(array, size);
    int dividend = 1;
    for (i = 0; i < maxLength; ++i) {
        distributeNumbers(array, size, bucket, dividend);
        rearrangeArray(array, size, bucket);
        dividend *= 10;
        // printArray(array, size);
    }
}

//获取数组array中最大数的长度（位数）
int getMaxLength(int array[], int size)
{
    assert(array != NULL && size > 0);

    int max = array[0];
    int i = 0;
    for (i = 1; i < size; ++i) {
        if (max < array[i]) {
            max = array[i];
        }
    }

    int length = 1;
    while ((max /= 10) != 0) {
        ++length;
    }
    return length;
}

//把数组array中的数放到对应的桶中,桶的底层是用链式队列实现的
void distributeNumbers(int array[], int size, struct Queue bucket[], int dividend)
{
    assert(array != NULL && size > 0 && bucket != NULL && dividend > 0);

    int radixValue = 0;
    struct Node* node;
    int i = 0;
    for (i = 0; i < size; ++i) {
        //把array[i]放到下标为radixValue的桶中
        radixValue = (array[i] / dividend) % 10;
        node = (struct Node*) malloc(sizeof(struct Node));
        node->value = array[i];
        node->next = NULL;
        if (bucket[radixValue].head == NULL) {
            bucket[radixValue].head = node;
            bucket[radixValue].tail = node;
        }
        else {
            bucket[radixValue].tail->next = node;
            bucket[radixValue].tail = node;
        }
    }
}

//把桶0..9中的数按放入桶中的先后次序放回到数组array中
void rearrangeArray(int array[], int size, struct Queue bucket[])
{
    assert(array != NULL && size > 0 && bucket != NULL);

    struct Node* pointer = NULL;
    int arrayIndex = 0;
    int listIndex = 0;
    for (listIndex = 0; listIndex < RADIXCOUNT; ++listIndex) {
        while (bucket[listIndex].head != NULL) {
            array[arrayIndex++] = bucket[listIndex].head->value;
            pointer = bucket[listIndex].head;
            bucket[listIndex].head = bucket[listIndex].head->next;
            free(pointer);
        }
    }
}

void printArray(int array[], int size)
{
    assert(array != NULL && size > 0);

    int i = 0;
    for (i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n\n");
}

//判断数组array是否已经是有序的
void isSorted(int array[], int size)
{
    assert(array != NULL && size > 0);

    int unsorted = 0;
    int i = 0;
    for (i = 1; i < size; ++i) {
        if (array[i] < array[i - 1]) {
            unsorted = 1;
            break;
        }
    }

    if (unsorted) {
        printf("the array is unsorted!\n");
    }
    else {
        printf("the array is sorted!\n");
    }
}