#pragma once

#include "types.h"

typedef struct DynamicArray {
    size_t length;
    size_t capacity;
    size_t itemSize;
    void *items;
} DynamicArray;

DynamicArray *dynamicArrayCreate(size_t itemSize, size_t initialCapacity);
void dynamicArrayDestroy(DynamicArray *pArray);
size_t dynamicArrayAppend(DynamicArray *pArray, void *pItem);
void *dynamicArrayGet(DynamicArray *pArray, size_t index);
u8 dynamicArrayRemove(DynamicArray *pArray, size_t index);
