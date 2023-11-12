#pragma once

#include <stdlib.h>
#include <stdio.h>

#define ERROR(...) { fprintf(stderr, __VA_ARGS__);; }
#define ERROR_EXIT(...) { fprintf(stderr, __VA_ARGS__); exit(1); }
#define ERROR_RETURN(R, ...) { fprintf(stderr, __VA_ARGS__); return R; }
#define ERROR_RETURN_VOID(...) { fprintf(stderr, __VA_ARGS__); return; }
