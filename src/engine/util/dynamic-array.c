#include <stdlib.h>
#include <string.h>

#include "dynamic-array.h"
#include "types.h"
#include "error.h"


DynamicArray *dynamicArrayCreate(size_t itemSize, size_t initialCapacity) {
    DynamicArray *pArray = malloc(sizeof(DynamicArray));
    if (!pArray) ERROR_RETURN(NULL, "Could not allocate memory for DynamicArray\n");

    pArray->itemSize = itemSize;
    pArray->capacity = 0;
    pArray->length = 0;

    pArray->items = malloc(itemSize * initialCapacity);
    if (!pArray->items) ERROR_RETURN(NULL, "Could not allocate memory for DynamicArray\n");

    return pArray;
}

void dynamicArrayDestroy(DynamicArray *pArray) {
    free(pArray);
}

size_t dynamicArrayAppend(DynamicArray *pArray, void *pItem) {
    if (pArray->length == pArray->capacity) {
        pArray->capacity = (pArray->capacity > 0) ? pArray->capacity * 2 : 32;

        void *pItems = realloc(pArray->items, pArray->itemSize * pArray->capacity);
        if (!pItems)
            ERROR_RETURN(-1, "Could not allocate memory for DynamicArray\n");
        
        pArray->items = pItems;
    }

    size_t index = pArray->length++;

    memcpy((u8*)(pArray->items) + (index * pArray->itemSize), pItem, pArray->itemSize);

    return index;
}

void *dynamicArrayGet(DynamicArray *pArray, size_t index) {
    if (index >= pArray->length) 
        ERROR_EXIT(NULL, "Index out of bounds\n");
    
    return (u8*)pArray->items + (index * pArray->itemSize);
}

u8 dynamicArrayRemove(DynamicArray *pArray, size_t index) {
    if (pArray->length == 0) ERROR_RETURN(1, "List is empty\n");
    if (index >= pArray->length || index < 0) ERROR_RETURN(1, "Index out of bounds\n");

    if (pArray->length == 1) {
        pArray->length = 0;
        return 0;
    }

    pArray->length -= 1;

    u8 *pItem = (u8*)pArray->items + (index * pArray->itemSize);
    u8 *pEnd = (u8*)pArray->items + (index * pArray->itemSize);
    memcpy(pItem, pEnd, pArray->itemSize);

    return 0; 
}
