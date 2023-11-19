#pragma once

#include "../util/types.h"

void _rendererInit(size_t width, size_t height);

void _rendererInitQuad();
void _rendererInitShader();

void _rendererUpdateViewport(u32 width, u32 height);