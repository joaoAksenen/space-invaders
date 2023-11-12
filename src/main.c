#include "stdio.h"
#include "engine/engine.h"

#include "engine/util/dynamicarray.h"

void printDA(DynamicArray *da) {
    printf("[ ");

    for (size_t i = 0; i < da->length; i++) {
        printf("%d ", ((i32*)da->items)[i]);
    }

    printf("]\n");
}

int main() {
    // bool init = engineInit("Space Invaders");
    // if (!init) return -1;

    // engineMainloop();
    // engineTerminate();

    DynamicArray* da = dynamicArrayCreate(sizeof(i32), 2);

    printf("Initial (len=%zu, cap=%zu): ", da->length, da->capacity);
    printDA(da);

    i32 data = 8;
    dynamicArrayAppend(da, &data);

    data = 32;
    dynamicArrayAppend(da, &data);

    printf("After insertions (len=%zu, cap=%zu): ", da->length, da->capacity);
    printDA(da);

    dynamicArrayRemove(da, 1);
    printf("After removal (len=%zu, cap=%zu): ", da->length, da->capacity);
    printDA(da);

    dynamicArrayDestroy(da);
    
    return 0;
}