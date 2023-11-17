#include <GLFW/glfw3.h>

#include "time-internal.h"
#include "time.h"

static f32 delta;          
static f32 now;            // Time of the current frame
static f32 last;           // Time of the last frame

static f32 frameLast;      // last time the FPS was computated
static f32 frameTime;      // how long the current frame took

static u32 frameRate;      // Frames Per Second (FPS)
static u32 frameCount;     

void _timeInit(u32 _frameRate) {
    glfwWindowHint(GLFW_REFRESH_RATE, _frameRate);
    frameRate = _frameRate;
}

f32 _timeUpdate() {
    now = (f32) glfwGetTime();
    delta = now - last;
    last = now;

    frameCount++;

    if (now - frameLast >= 1) {
        frameRate = frameCount;
        frameCount = 0;
        frameLast = now;
    }

    return delta;
}

f32 engineTimeGetDeltaTime() {
    return delta;
}

u32 engineTimeGetFrameRate() {
    return frameRate;
}

f32 engineTimeGetTime() {
    return now;
}