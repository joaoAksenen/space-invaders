#include "engine/engine.h"

void onUpdate(f32 delta) {
    engineRendererDrawQuad(100, 100, 100, 100);
}

int main() {
    bool init = engineInit("Space Invaders");
    if (!init) return -1;

    engineSceneCreate("scene 1", onUpdate, NULL, NULL);

    engineMainloop();
    engineTerminate();

    return 0;
}