#include "main.h"

extern void ibm_handlewinkey(int key, int up);

static const char *g_windowClass = "hexends32";
static const char *g_windowTitle = "HexenDS Win32 Test Stub";

HINSTANCE g_hInstance = 0;
int g_iDisplayWidth = 0;
int g_iDisplayHeight = 0;

static HWND g_hWnd = 0;
static int g_fullScreen = 0;

static HDC g_hDC = 0;
static HGLRC g_hRC = 0;

//same functionality as gluPerspective
void GL_CalcPerspective(float fovy, float aspect, float zNear, float zFar)
{
	float xmin, xmax, ymin, ymax;

	ymax = zNear * (float)tan(fovy * PI / 360.0f);
	ymin = -ymax;

	xmin = ymin * aspect;
	xmax = ymax * aspect;

	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

//swapbuffers
void Window_SwapBuffers(void)
{
	SwapBuffers(g_hDC);
}

//create the device context, etc
BOOL Window_GLInit(HWND wnd, int w, int h)
{
	int pixelFormat;

	if (!g_hDC)
	{ //otherwise we're just sizing the viewport.
		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24, //depth bits
			24, //stencil bits
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		g_hWnd = wnd;
		g_hDC = GetDC(g_hWnd);

		pixelFormat = ChoosePixelFormat(g_hDC, &pfd);
		if (!pixelFormat)
		{
			return FALSE;
		}

		if (!SetPixelFormat(g_hDC, pixelFormat, &pfd))
		{
			return FALSE;
		}

		g_hRC = wglCreateContext(g_hDC);
		if (!g_hRC)
		{
			return FALSE;
		}
	}

	if (!wglMakeCurrent(g_hDC, g_hRC))
	{
		return FALSE;
	}

	g_iDisplayWidth = w;
	g_iDisplayHeight = h;

	if (g_iDisplayWidth <= 0)
	{
		assert(0);
		g_iDisplayWidth = 1;
	}
	if (g_iDisplayHeight <= 0)
	{
		assert(0);
		g_iDisplayHeight = 1;
	}
	glViewport(0, 0, g_iDisplayWidth, g_iDisplayHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GL_CalcPerspective(45.0f, (float)g_iDisplayWidth/(float)g_iDisplayHeight, Z_NEAR, Z_FAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set default states
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return TRUE;
}

//called to close window
void Window_Close(void)
{
	if (g_fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	//clean up the rendering context
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(g_hRC);
	g_hRC = 0;

	if (g_hWnd)
	{
		DestroyWindow(g_hWnd);
		g_hWnd = 0;
	}

	UnregisterClass(g_windowClass, g_hInstance);
}

static int MapH2Key(int winKey)
{
#define	KEY_RIGHTARROW		0xae
#define	KEY_LEFTARROW		0xac
#define	KEY_UPARROW			0xad
#define	KEY_DOWNARROW		0xaf
#define	KEY_ESCAPE			27
#define	KEY_ENTER			13
#define	KEY_F1				(0x80+0x3b)
#define	KEY_F2				(0x80+0x3c)
#define	KEY_F3				(0x80+0x3d)
#define	KEY_F4				(0x80+0x3e)
#define	KEY_F5				(0x80+0x3f)
#define	KEY_F6				(0x80+0x40)
#define	KEY_F7				(0x80+0x41)
#define	KEY_F8				(0x80+0x42)
#define	KEY_F9				(0x80+0x43)
#define	KEY_F10				(0x80+0x44)
#define	KEY_F11				(0x80+0x57)
#define	KEY_F12				(0x80+0x58)
#define KEY_TAB				9
#define	KEY_RSHIFT			(0x80+0x36)
#define	KEY_RCTRL			(0x80+0x1d)
#define	KEY_RALT			(0x80+0x38)

	if (winKey == VK_UP)
	{
		return KEY_UPARROW;
	}
	if (winKey == VK_DOWN)
	{
		return KEY_DOWNARROW;
	}
	if (winKey == VK_LEFT)
	{
		return KEY_LEFTARROW;
	}
	if (winKey == VK_RIGHT)
	{
		return KEY_RIGHTARROW;
	}
	if (winKey == VK_RETURN)
	{
		return KEY_ENTER;
	}
	if (winKey == VK_ESCAPE)
	{
		return KEY_ESCAPE;
	}
	if (winKey == VK_F11)
	{
		return KEY_F11;
	}
	if (winKey == VK_TAB)
	{
		return KEY_TAB;
	}
	if (winKey == VK_RCONTROL || winKey == VK_CONTROL)
	{
		return KEY_RCTRL;
	}
	if (winKey == VK_RSHIFT || winKey == VK_SHIFT)
	{
		return KEY_RSHIFT;
	}
	if (winKey == VK_OEM_2)
	{
		return '/';
	}
	if (winKey == 77) {
		return 'm';
	}
	if (winKey == 66) {
		return 'b';
	}
	if (winKey == 76)
	{
		return 'l';
	}
	if (winKey == 221)
	{
		return ']';
	}

	return winKey;
}

LRESULT CALLBACK Window_Callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_ACTIVATE:
		{
			return 0;
		}

		case WM_CLOSE:
		{
			Window_Close();
			return 0;
		}

		case WM_KEYDOWN:
		{
			ibm_handlewinkey(MapH2Key(wParam), 0);
			return 0;
		}
		case WM_KEYUP:
		{
			ibm_handlewinkey(MapH2Key(wParam), 1);
			return 0;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//create the main window
HWND *Window_Create(int width, int height, int bits, int fullScreen)
{
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;
	WNDCLASS	wc;

	WindowRect.left = 0;
	WindowRect.right= width;
	WindowRect.top = 0;
	WindowRect.bottom = height;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)Window_Callback;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= g_hInstance;
	wc.hIcon			= 0;//LoadIcon(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= g_windowClass;

	if (!RegisterClass(&wc))
	{
		return 0;
	}
	
	if (fullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= width;
		dmScreenSettings.dmPelsHeight	= height;
		dmScreenSettings.dmBitsPerPel	= bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			fullScreen = FALSE;
		}
	}

	g_fullScreen = fullScreen;

	if (fullScreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_CAPTION|WS_SYSMENU;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	g_hWnd = CreateWindowEx(	dwExStyle,
								g_windowClass,
								g_windowTitle,
								dwStyle |
								WS_CLIPSIBLINGS |
								WS_CLIPCHILDREN,
								0, 0,
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,
								NULL,
								g_hInstance,
								NULL);
	if (!g_hWnd)
	{
		Window_Close();
		return 0;
	}

	if (!Window_GLInit(g_hWnd, width, height))
	{
		Window_Close();
		return 0;
	}

	return &g_hWnd;
}