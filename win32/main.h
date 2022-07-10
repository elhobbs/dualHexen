#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <assert.h>
#include <gl/gl.h>

#define GL_CLAMP_TO_EDGE					0x812F
#define GL_TEXTURE_RECTANGLE_NV				0x84F5

#define PI						3.14159265358979323846

#define Z_NEAR					16.0f
#define Z_FAR					32768.0f

extern HINSTANCE g_hInstance;
extern int g_iDisplayWidth;
extern int g_iDisplayHeight;
