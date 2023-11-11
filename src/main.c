#include "stdio.h"
#include "engine/engine.h"

int main() {
    bool init = engineInit("Space Invaders");
    if (!init) return -1;

    engineMainloop();
    engineTerminate();

    return 0;
}