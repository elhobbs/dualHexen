//HexenDS
//Win32 stub module for testing
#include "main.h"

#ifdef _DS_RESOLUTION
#define	H2SCREENWIDTH		256
#define	H2SCREENHEIGHT		192
#define H2DRAWSCREENWIDTH	320
#define H2DRAWSCREENHEIGHT	200
#else
#define	H2SCREENWIDTH		320
#define	H2SCREENHEIGHT		200
#define H2DRAWSCREENWIDTH	320
#define H2DRAWSCREENHEIGHT	200
#endif


HWND *g_stubWnd = NULL;

extern void ibm_main(int argc, char **argv);

extern HWND *Window_Create(int width, int height, int bits, int fullScreen);
extern void Window_SwapBuffers(void);

extern byte softPCScreenPal[256*3];
#ifdef _DS_RESOLUTION
extern byte softPCScreenBuffer[256*192];
extern byte softSubPCScreenBuffer[320*200];
#else
extern byte softPCScreenBuffer[320*200];
#endif

int g_glScreenTex = 0;
int g_glSubScreenTex = 0;

void *ds_malloc(int size)
{
	return malloc(size);
}

void ds_free(void *ptr)
{
	free(ptr);
}

#pragma pack(1)
//standard 24-bit bitmap pixel
typedef struct
{
	BYTE			c[3];
} bitmapPixel_t;

//standard bitmap header
typedef struct bitmapHeader_s
{
	BYTE			ident[2];
	DWORD			size;
	WORD			reserved1;
	WORD			reserved2;
	DWORD			bitmapDataOffset;
} bitmapHeader_t;

//standard bitmap info header
typedef struct bitmapInfo_s
{
	DWORD			bitmapHeaderSize;
	LONG			width;
	LONG			height;
	WORD			planes;
	WORD			bpp;
	DWORD			compression;
	DWORD			bitmapDataSize;
	LONG			hRes;
	LONG			vRes;
	DWORD			colors;
	DWORD			importantColors;
} bitmapInfo_t;

//p is the beginning of pixel data
typedef struct bitmap_s
{
	bitmapHeader_t	h;
	bitmapInfo_t	i;
	bitmapPixel_t	p;
} bitmap_t;

typedef struct rgbPixel_s
{
	BYTE			rgb[3];
} rgbPixel_t;

#pragma pack(4) //restore 4-byte alignment

//debugging routine - writes a buffer to a bitmap
static void BMPWriteBufferDebug(rgbPixel_t *buf, int w, int h)
{
	const char *name = "e:\\other\\buffer_test.bmp";
	int i;
	int cW;
	int cH;
	int row;
	int col;
	bitmapPixel_t *bp;
	rgbPixel_t *p;
	int size = w*h;
	int pixels = (w*h)*sizeof(bitmapPixel_t);
	int bSize = sizeof(bitmap_t)-sizeof(bitmapPixel_t);
	int fileHandle;

	bitmap_t *bmp = (bitmap_t *)malloc(bSize+pixels);

	memset(bmp, 0, bSize+pixels);

	//some hardcoded info for our 24-bit output bitmap
	bmp->h.ident[0] = 'B';
	bmp->h.ident[1] = 'M';
	bmp->h.size = bSize+pixels;
	bmp->h.bitmapDataOffset = bSize;
	bmp->i.bitmapHeaderSize = bSize-sizeof(bitmapHeader_t);
	bmp->i.width = w;
	bmp->i.height = h;
	bmp->i.planes = 1;
	bmp->i.bpp = 24;
	bmp->i.compression = 0;
	bmp->i.bitmapDataSize = pixels;
	bmp->i.hRes = 0;
	bmp->i.vRes = 0;
	bmp->i.colors = 0;
	bmp->i.importantColors = 0;

	i = 0;
	cW = w;
	cH = h;
	row = cH;
	col = cW;
	bp = &bmp->p;
	p = buf;
	p += (cW*row)-cW;

	while (i < (cW*cH))
	{
		bp->c[0] = p->rgb[2];
		bp->c[1] = p->rgb[1];
		bp->c[2] = p->rgb[0];

		p++;
		col--;
		if (col < 1)
		{
			col = cW;
			row--;
			p = buf;
			p += (cW*row)-cW;
		}
		bp++;
		i++;
	}

	fileHandle = _open(name, _O_WRONLY|_O_BINARY|_O_CREAT, _S_IWRITE);

	if (fileHandle == -1)
	{
		return;
	}

	_write(fileHandle, bmp, bSize+pixels);

	_close(fileHandle);

	free(bmp);
}

static rgbPixel_t g_debugPixBuffer[320*200];
void Win32_DebugPCWriteScreen(void)
{
	int i = 0;
	while (i < (H2SCREENWIDTH*H2SCREENHEIGHT))
	{
		g_debugPixBuffer[i].rgb[0] = softPCScreenPal[(softPCScreenBuffer[i]*3)];
		g_debugPixBuffer[i].rgb[1] = softPCScreenPal[(softPCScreenBuffer[i]*3)+1];
		g_debugPixBuffer[i].rgb[2] = softPCScreenPal[(softPCScreenBuffer[i]*3)+2];
		i++;
	}

	BMPWriteBufferDebug(g_debugPixBuffer, H2SCREENWIDTH, H2SCREENHEIGHT);
}

static void CreateScreenTexture(void)
{
	glGenTextures(1, &g_glScreenTex);

	glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_glScreenTex);
	glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, H2SCREENWIDTH, H2SCREENHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &g_glSubScreenTex);

	glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_glSubScreenTex);
	glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, H2DRAWSCREENWIDTH, H2DRAWSCREENHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

static void UpdateScreenTexture(void)
{
	int i = 0;
	while (i < (H2SCREENWIDTH*H2SCREENHEIGHT))
	{
		g_debugPixBuffer[i].rgb[0] = softPCScreenPal[(softPCScreenBuffer[i]*3)];
		g_debugPixBuffer[i].rgb[1] = softPCScreenPal[(softPCScreenBuffer[i]*3)+1];
		g_debugPixBuffer[i].rgb[2] = softPCScreenPal[(softPCScreenBuffer[i]*3)+2];
		i++;
	}

	glEnable(GL_TEXTURE_RECTANGLE_NV); 
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_glScreenTex);
	glTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, H2SCREENWIDTH, H2SCREENHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, g_debugPixBuffer); 
	//glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, H2SCREENWIDTH, H2SCREENHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, g_debugPixBuffer);
	glDisable(GL_TEXTURE_RECTANGLE_NV);
}

static void UpdateSubScreenTexture(void)
{
	int i = 0;
	while (i < (H2DRAWSCREENWIDTH*H2DRAWSCREENHEIGHT))
	{
		g_debugPixBuffer[i].rgb[0] = softPCScreenPal[(softSubPCScreenBuffer[i]*3)];
		g_debugPixBuffer[i].rgb[1] = softPCScreenPal[(softSubPCScreenBuffer[i]*3)+1];
		g_debugPixBuffer[i].rgb[2] = softPCScreenPal[(softSubPCScreenBuffer[i]*3)+2];
		i++;
	}

	glEnable(GL_TEXTURE_RECTANGLE_NV); 
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_glSubScreenTex);
	glTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, H2DRAWSCREENWIDTH, H2DRAWSCREENHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, g_debugPixBuffer); 
	//glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, H2SCREENWIDTH, H2SCREENHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, g_debugPixBuffer);
	glDisable(GL_TEXTURE_RECTANGLE_NV);
}

//ortho view matrix
static void GL_ToOrtho(void)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, g_iDisplayWidth, g_iDisplayHeight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

//perspective view matrix
static void GL_BackToPerspective(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Win32_WindowDraw(int subchanged)
{
	const int detailMod = 1;
	Window_SwapBuffers();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	UpdateScreenTexture();
	if (subchanged)
	{
		UpdateSubScreenTexture();
	}

	glEnable(GL_TEXTURE_RECTANGLE_NV); 

	GL_ToOrtho();

	//draw the texture quads
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_glScreenTex);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, (float)H2SCREENHEIGHT); 
		glVertex2f(0.0f, (float)g_iDisplayHeight/2);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);

		glTexCoord2f((float)H2SCREENWIDTH, 0.0f);
		glVertex2f((float)g_iDisplayWidth*detailMod, 0.0f);

		glTexCoord2f((float)H2SCREENWIDTH, (float)H2SCREENHEIGHT);
		glVertex2f((float)g_iDisplayWidth*detailMod, (float)g_iDisplayHeight/2);
	glEnd();

	glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_glSubScreenTex);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, (float)H2DRAWSCREENHEIGHT); 
		glVertex2f(0.0f, (float)g_iDisplayHeight);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, (float)g_iDisplayHeight/2);

		glTexCoord2f((float)H2DRAWSCREENWIDTH, 0.0f);
		glVertex2f((float)g_iDisplayWidth, (float)g_iDisplayHeight/2);

		glTexCoord2f((float)H2DRAWSCREENWIDTH, (float)H2DRAWSCREENHEIGHT);
		glVertex2f((float)g_iDisplayWidth, (float)g_iDisplayHeight);
	glEnd();

	GL_BackToPerspective();

	glDisable(GL_TEXTURE_RECTANGLE_NV); 

	Sleep(5); //keep it from eating the system
}

void Win32_WindowUpdate(void)
{
	MSG msg;

	if (!*g_stubWnd)
	{
		exit(1);
	}

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

int Win32_GetH2Tics(void)
{
	return (int)((float)GetTickCount()*0.035f);
}

int HexenDivInt(int a, int b)
{
	return a/b;
}

unsigned int HexenDivIntU(unsigned int a, unsigned int b)
{
	return a/b;
}

extern int detailLevel;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInstance = hInstance;
	g_stubWnd = Window_Create(640, 960, 32, 0);
	if (!g_stubWnd)
	{
		return 0;
	}

	CreateScreenTexture();

	ShowWindow(*g_stubWnd, TRUE);

	detailLevel = 1;

	ibm_main(0, NULL);
	return 0;
}
