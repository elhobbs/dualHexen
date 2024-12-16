#include "ds_defs.h"
#include "h2def.h"
#include <ctype.h>

#ifdef ARM9
#include <nds.h>
#include "keyboard.h"
#include "spleen5x8.h"
#endif

extern boolean	automapactive;
static boolean	automapactive_last = true;

int keyboard_resized = 0;

typedef struct {
	int x, y, type, dx, key;
	char *text, *shift_text;
} sregion_t;

void keyboard_draw_region(sregion_t *region, int index, byte c);


int keyboard_visible_last = 0;
int	keyboard_visible = 2; //0=hidden,1=fullsize,2=mini - numbers only

#define KEYBOARD_FULL_VOFS 100
#define KEYBOARD_MINI_VOFS 0
#define KEYBOARD_HOFS 0

#define KEY_WIDTH(_n) (5*(_n)+6)


static char key_buttons[] = "1234567890";

static sregion_t key_button_array[] = {
	{ 32, 0, 0, 0, 0, key_buttons, key_buttons },
	{ 240, 0, 6, 0, 0x200, 0 },
};
static int key_button_array_count = sizeof(key_button_array) / sizeof(sregion_t);

static char key_row_1[] = "~1234567890-=";
static char key_row_1_shift[] = "`!@#$%^&*()_+";

static char key_row_2[] = "qwertyuiop[]\\";
static char key_row_2_shift[] = "QWERTYUIOP{}|";

static char key_row_3[] = "asdfghjkl;'";
static char key_row_3_shift[] = "ASDFGHJKL:\"";

static char key_row_4[] = "zxcvbnm,./";
static char key_row_4_shift[] = "ZXCVBNM<>?";

static char key_tab[] = "Tab";
static char key_caps[] = "Caps";
static char key_shift[] = "Shift";
static char key_ctrl[] = "Ctrl";
static char key_alt[] = "Alt";
static char key_space[] = "        SPACE        ";
static char key_backspace[] = "Bksp";
static char key_return[] = "Rtrn";

#define KEY_TAB 9
#define KEY_CAPSLOCK 0xba

static sregion_t key_array[] = {
	//row 1
	{ 0,			0 * 16,					0,	0,		0,				key_row_1,		key_row_1_shift },
	{ 0,			0 * 16,					1,	0,		KEY_BACKSPACE,	key_backspace },
	//row 2
	{ 0,			1 * 16,					1,	0,		KEY_TAB,		key_tab },
	{ 0,			1 * 16,					0,	0,		0,				key_row_2,	key_row_2_shift},
	//row 3
	{ 0,			2 * 16,					1,	0,		KEY_CAPSLOCK,	key_caps },
	{ 0,			2 * 16,					0,	0,		0,				key_row_3, key_row_3_shift },
	{ 0,			2 * 16,					1,	0,		KEY_ENTER,		key_return },
	//row 4
	{ 0,			3 * 16,					1,	0,		KEY_RSHIFT,		key_shift },
	{ 0,			3 * 16,					0,	0,		0, key_row_4,	key_row_4_shift },
	{ 0,			3 * 16,					1,	0,		KEY_LSHIFT,		key_shift },
	{ 256-15*2+1,	3 * 16,					2,	0,		KEY_UPARROW,	0 },
	//row 5
	{ 0,			4 * 16,					1,	0,		KEY_LCTRL,		key_ctrl },
	{ 0,			4 * 16,					1,	0,		KEY_LALT,		key_alt },
	{ 0,			4 * 16,					1,	0,		' ',			key_space },
	{ 0,			4 * 16,					1,	0,		KEY_RALT,		key_alt },
	{ 0,			4 * 16,					1,	0,		KEY_RCTRL,		key_ctrl },
	{ 256-15*3+1,	4 * 16,					3,	0,		KEY_LEFTARROW,	0 },
	{ 256-15*2+1,	4 * 16,					4,	0,		KEY_DOWNARROW,	0 },
	{ 256-15*1+1,	4 * 16,					5,	0,		KEY_RIGHTARROW, 0 },
	{ 240,			-KEYBOARD_FULL_VOFS,	6,	0,		0x200,			0 }
};
static int key_array_count = sizeof(key_array) / sizeof(sregion_t);

#define RGB8_to_565(r,g,b)  (((b)>>3)&0x1f)|((((g)>>2)&0x3f)<<5)|((((r)>>3)&0x1f)<<11)

static byte keyboard_fg = 2 * 16;
static byte keyboard_bg = 13 * 16 + 5;
static byte keyboard_fg_am = 2 * 16;
static byte keyboard_bg_am = 13 * 16 + 5;

static int keyboard_vofs;
static int keyboard_hofs;

static sregion_t *key_touching = 0;
static int key_touching_index = -1;
static int key_in_touch = 0;
static int last_in_touch = 0;

static int last_index = -1;
static sregion_t *last_touching = 0;

static int key_down = 0;
static int key_in_shift = 0;
static int key_in_ctrl = 0;
static int key_in_alt = 0;
static int key_in_caps = 0;

static int key_tap_time = -1;

static char key_arrow[12][12] = {
	{ 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 7, 7, 7, 7, 7, 0, 0, 0 },
	{ 0, 0, 0, 7, 7, 7, 7, 7, 7, 0, 0, 0 },
	{ 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 0, 0 },
	{ 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0 },
	{ 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0 },
	{ 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0 },
	{ 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
	{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, }
};

byte key_arrow_bitmaps[4][8] = {
	{ 0x00, 0x10, 0x38, 0x7C, 0x10, 0x10, 0x10, 0x00 }, //up
	{ 0x00, 0x10, 0x10, 0x10, 0x7C, 0x38, 0x10, 0x00 }, //down
	{ 0x00, 0x00, 0x10, 0x30, 0x7E, 0x30, 0x10, 0x00 }, //right
	{ 0x00, 0x00, 0x08, 0x0C, 0x7E, 0x0C, 0x08, 0x00 } //left
};


void keyboard_init()
{
	int i, len;
	int count = sizeof(key_array) / sizeof(sregion_t);
	int x_offs[10];

	memset(x_offs, 0, sizeof(x_offs));

	for (i = 0; i<count; i++)
	{
		switch (key_array[i].type) {
		case 0:
			len = strlen(key_array[i].text) * 16;
			break;
		case 1:
			len = KEY_WIDTH(strlen(key_array[i].text)) + 1;
			break;
		default:
			len = 16;
		}
		key_array[i].dx = len;
		if (key_array[i].y >= 0) {
			int row = key_array[i].y >> 4;
			if (key_array[i].x == 0) {
				key_array[i].x = x_offs[row];
			}
			x_offs[row] += len;
		}
	}
	key_button_array[0].dx = strlen(key_button_array[0].text) * 16;
	key_button_array[1].dx = 16;
	
	keyboard_vofs = KEYBOARD_FULL_VOFS;
	keyboard_hofs = KEYBOARD_HOFS;
	if (keyboard_visible == 2) {
		keyboard_vofs = KEYBOARD_MINI_VOFS;
	}
}

void keyboard_mark(sregion_t *region, int index, int in_touch) {
	last_touching = key_touching;
	last_index = key_touching_index;
	last_in_touch = key_in_touch;

	key_touching = region;
	key_touching_index = index;
	key_in_touch = in_touch;
	//check for shift and caps on keydown so that they get colored correctly
	if (key_touching && key_touching != last_touching) {
		if (key_touching->type == 6 || key_touching->type == 1) {

			switch (key_touching->key) {
			case 0x200:
				//change the keyboard layout
				keyboard_visible++;
				if (keyboard_visible > 2) {
					keyboard_visible = 1;
				}
				//clear touch to avoid layout change issues
				key_touching = last_touching = 0;
				key_touching_index = last_index = -1;
				//force full redraw
				SB_state = -1;
				UpdateState |= I_FULLSCRN;
				keyboard_resized = 1;
				break;
			case KEY_RSHIFT:
				//force whole refresh
				keyboard_visible_last = -1;
				key_in_shift = key_in_shift ? 0 : 1;
				break;
			case KEY_CAPSLOCK:
				//force whole refresh
				keyboard_visible_last = -1;
				key_in_caps = key_in_caps ? 0 : 1;
				break;
			}
		}
	}
	//draw regions here to avoid layout change issues
	keyboard_draw_region(last_touching, last_index, automapactive ? keyboard_fg_am : keyboard_fg);
	keyboard_draw_region(key_touching, key_touching_index, automapactive ? keyboard_bg_am : keyboard_bg);
	UpdateState |= I_FULLSCRN;
}

int keyboard_scankeys()
{
	static sregion_t *region;
	int keys;

	int i, len, x, y, pos;
	int count;
	int key;

	if (keyboard_visible == 0)
	{
		key_down = 0;
		keyboard_mark(0, -1, 0);
		return 0;
	}

	x = y = -1;

#ifdef ARM9
	touchPosition	touch = { 0, 0 };
	
	keys = keysHeld();

	if (keys & KEY_TOUCH)
	{
		//if we are already touching something then bail
		//this causes only the initial key to register
		//and a single key per touch
		if (key_in_touch) {
			return key_down;
		}

		touchRead(&touch);
		x = touch.px - keyboard_hofs;
		y = touch.py - keyboard_vofs;
		if (y < -keyboard_vofs)
		{
			keyboard_mark(0, -1, 1);
			return key_down;
		}
		//iprintf("Touch: %d %d\n",x,y);
	}
	else
	{
		key_down = 0;
		keyboard_mark(0, -1, 0);
		return key_down;
	}

	region = key_array;
	count = key_array_count;
	if (keyboard_visible == 2)
	{
		region = key_button_array;
		count = key_button_array_count;
	}

	for (i = 0; i<count; i++)
	{
		if (y < region[i].y || y >(region[i].y + 16))
		{
			continue;
		}
		len = region[i].dx;
		if (x < region[i].x || x >(region[i].x + len))
		{
			continue;
		}
		if (region[i].type == 0)
		{
			pos = (x - region[i].x) / 16;
		}
		else
		{
			pos = 0;
		}

		keyboard_mark(&region[i], pos, 1);
		if (!key_touching) {
			break;
		}
		switch (key_touching->type)
		{
		case 0:
			key_down = key_in_shift ? key_touching->shift_text[key_touching_index] : (key_in_caps ? toupper(key_touching->text[key_touching_index]) : key_touching->text[key_touching_index]);
			if (key_in_shift) {
				//force a full keyboard refresh
				keyboard_visible_last = -1;
				key_in_shift = 0;
			}
			break;
		default:
			key_down = key_touching->key;
			break;
		}

		return key_down;
	}

	//no key pressed but still in key touch
	keyboard_mark(0, -1, 1);
	return key_down;
#else
	return 0;
#endif
}

void keyboard_input() {
	static int key_last = 0;
	static int key = 0;
	//event_t event;

	void RegisterDSKey(unsigned short key, unsigned int up);

	key_last = key;
	key = keyboard_scankeys();
	//iprintf("key: %d\n", key);
	if (key_last != 0 && key_last != key)
	{
		//iprintf("key up: %d %c\n", key_last, key_last);
		//event.type = ev_keyup;
		//event.data1 = key_last;
		//D_PostEvent(&event);
		RegisterDSKey(key_last, 1);
	}

	if (key != 0 && key != key_last)
	{
		//iprintf("key down: %d %c\n", key, key);
		//event.type = ev_keydown;
		//event.data1 = key;
		//D_PostEvent(&event);
		RegisterDSKey(key, 0);
	}
}

#ifdef _3DS
extern const u8 default_font_bin[];
static u16* keyboard_screen;
#endif

#ifdef ARM9
extern byte *ds_bottom_screen;
extern const u8 default_font_bin[];
byte* keyboard_screen;
#endif

#ifdef WIN32
const byte default_font_bin[256*64];
byte ds_bottom_screen[400 * 240];
static byte *keyboard_screen = ds_bottom_screen;
#endif

extern byte* subscreen;


static int FontABaseLump = -1;


static void draw_patch(int x, int y, patch_t* patch)
{
#if 1
	int count;
	int col;
	column_t* column;
	byte* desttop;
	byte* dest;
	byte* source;
	int w;

	y -= SHORT(patch->topoffset);
	x -= SHORT(patch->leftoffset);
	if (x < 0 || x + SHORT(patch->width) > DRAWSCREENWIDTH || y < 0
		|| y + SHORT(patch->height) > DRAWSCREENHEIGHT)
	{
		//printf("x: %d y: %d\n", x, y);
		//printf("w: %d h: %d\n", patch->width, patch->height);
		//I_Error("Bad V_DrawPatch");
		return;
	}
	col = 0;
	desttop = keyboard_screen + y * DRAWSCREENWIDTH + x;
	w = SHORT(patch->width);
	for (; col < w; x++, col++, desttop++)
	{
		column = (column_t*)((byte*)patch + LONG(patch->columnofs[col]));
		// Step through the posts in a column
		while (column->topdelta != 0xff)
		{
			source = (byte*)column + 3;
			dest = desttop + column->topdelta * DRAWSCREENWIDTH;
			count = column->length;
			while (count--)
			{
				*dest = *source++;
				dest += DRAWSCREENWIDTH;
			}
			column = (column_t*)((byte*)column + column->length + 4);
		}
	}
#endif
}


void keyboard_draw_char_unused(int x, int y, int c, byte fg) {
	if (c < 33 || FontABaseLump == -1) {
		return;
	}
	patch_t* patch = W_CacheLumpNum(FontABaseLump + c - 33, PU_CACHE);
	draw_patch(x, y, patch);
}

void keyboard_draw_char2(int x, int y, int c, byte fg) {
	if (c < 0 || c > 256) return;

	byte* fontdata = default_font_bin + (32 * c);

	//if (currentConsole->flags & CONSOLE_UNDERLINE) b8 = 0xff;

	//if (currentConsole->flags & CONSOLE_CROSSED_OUT) b4 = 0xff;

	int i;
	byte* screen = &keyboard_screen[256 * y + x];

	for (i = 0; i < 32; i++) {
		byte b = *fontdata++;

		if (b & 0xf) {
			*screen = fg;
		}
		screen++;

		if (b & 0xf0) {
			*screen = fg;
		}
		screen++;
		if ((i & 0x3) == 0x3) {
			screen += (DRAWSCREENWIDTH - 8);
		}
	}

}

void keyboard_draw_char(int x, int y, int c, byte fg) {
	if (c < 0 || c > 256) return;

	byte* fontdata = default_font_bin + (8 * c);

	//if (currentConsole->flags & CONSOLE_UNDERLINE) b8 = 0xff;

	//if (currentConsole->flags & CONSOLE_CROSSED_OUT) b4 = 0xff;

	int i,j;
	byte* screen = &keyboard_screen[320 * y + x];

	for (i = 0; i < 8; i++) {
		byte b = *fontdata++;
		for (j = 0; j < 8; j++) {
			if ((b & (1 << j)) != 0) {
				*screen = fg;
			}
			screen++;
		}
		screen += (DRAWSCREENWIDTH - 8);
	}

}

void vline(int x, int y, int len, byte fg) {
	int i;
	byte *screen = &keyboard_screen[DRAWSCREENWIDTH *y + x];
	for (i = 0; i < len; i++) {
		*screen = fg;
		screen += DRAWSCREENWIDTH;
	}
}

void hline(int x, int y, int len, byte fg) {
	int i;
	byte*screen = &keyboard_screen[DRAWSCREENWIDTH * y + x];
	for (i = 0; i < len; i++) {
		*screen++ = fg;
	}
}

static void bitmap_draw(byte* bitmap, byte c, byte* line) {
	for (int yy = 0; yy < 8; yy++) {
		for (int xx = 0; xx < 5; xx++) {
			int set = bitmap[yy] & (0x80 >> xx);
			if (set) {
				line[xx] = 0;
			}
		}
		line += DRAWSCREENWIDTH;
	}
}

static void draw_arrow(int num, byte c, byte* line) {
	byte* bitmap = key_arrow_bitmaps[num];
	line += (5 * DRAWSCREENWIDTH) + 2;
	for (int yy = 0; yy < 8; yy++) {
		for (int xx = 0; xx < 8; xx++) {
			int set = bitmap[yy] & (0x80 >> xx);
			if (set) {
				line[xx] = 0;
			}
		}
		line += DRAWSCREENWIDTH;
	}
}

static void draw_key_button(int width, int c, char* line) {
	memset(line + 1, c, width - 2);
	line += DRAWSCREENWIDTH;
	for (int j = 0; j < 13; j++) {
		memset(line, c, width - 0);
		line += DRAWSCREENWIDTH;
	}
	memset(line + 1, c, width - 2);
	line += DRAWSCREENWIDTH;
}


static void key_draw(int x, int y, char* text, int width, byte c) {
	byte* buf = &keyboard_screen[x];
	byte* line = buf + (DRAWSCREENWIDTH * y);
	byte* bitmap;

	draw_key_button(width, c, line);

	while (*text) {
		line = buf + (DRAWSCREENWIDTH * (y + 5)) + 2;


		int k = key_in_caps ? toupper(*text) : *text;
		bitmap = &spleen5x8_data[(k - 32) * 8];
		bitmap_draw(bitmap, c, line + 1);

		if (width == 15) {
			break;
		}

		text++;
		buf += 5;
	}
}


void keyboard_draw_region(sregion_t *region, int index, byte c) {
	int len;
	char *ch;
	byte *screen;
	int j, k, pos;
	int x, y;
	byte* bitmap;

	//return;
	if (keyboard_visible == 0)
	{
		return;
	}

	if (region == 0) {
		return;
	}

	if (FontABaseLump == -1) {
		FontABaseLump = W_GetNumForName("FONTA_S") + 1;
	}

	//bail if the keyboard is set to redraw
	if (keyboard_visible != keyboard_visible_last) {
		return;
	}
	
	x = keyboard_hofs + region->x;
	y = keyboard_vofs + region->y;
	keyboard_screen = subscreen;// ds_bottom_screen;
	keyboard_screen += ((320 - 256) / 2);
	keyboard_screen += (320 * ((200 - 192) / 2));

	screen = &keyboard_screen[y * DRAWSCREENWIDTH + x];

	if (region->type == 0)
	{
		ch = key_in_shift ? region->shift_text : region->text;
		pos = 0;
		while (ch && *ch)
		{
			if (index == -1 || index == pos) {
#if 1
				key_draw(x, y, ch, 15, c);
#else
				//left/right sides
				vline(x, y + 1, 14, c);
				vline(x + 14, y + 1, 14, c);
				//top/bottom
				hline(x + 1, y + 1, 14, c);
				hline(x + 1, y + 14, 14, c);

				k = key_in_caps ? toupper(*ch) : *ch;
				keyboard_draw_char(x + 3, y + 4, k, c);
#endif
			}
			ch++;
			x += 16;
			pos++;
		}
	}
	else if (region->type == 1)
	{
		if ((region->key == KEY_CAPSLOCK && key_in_caps) || (region->key == KEY_RSHIFT && key_in_shift)) {
			c = automapactive ? keyboard_bg_am : keyboard_bg;
		}
		else if ((region->key == KEY_CAPSLOCK && !key_in_caps) || (region->key == KEY_RSHIFT && !key_in_shift)) {
			c = automapactive ? keyboard_fg_am : keyboard_fg;
		}
		ch = region->text;
		len = strlen(ch) * 5 + 6;

#if 1
		key_draw(x, y, ch, len, c);
#else
		//left/right sides
		vline(x, y + 1, 14, c);
		vline(x + len, y + 1, 14, c);
		//top/bottom
		hline(x + 1, y + 1, len, c);
		hline(x + 1, y + 14, len, c);

		while (ch && *ch)
		{
			keyboard_draw_char(x + 3, y + 4, *ch, c);
			ch++;
			x += 8;
		}
#endif
	}
	else if (region->type == 2)
	{
#if 1
		draw_key_button(14, c, screen);
		draw_arrow(0, 0, screen + 1);

#else
		for (j = 0; j<12; j++)
		{
			for (k = 0; k<12; k++)
			{
				if (key_arrow[j][11 - k])
					screen[k] = c;
			}
			screen += DRAWSCREENWIDTH;
		}
#endif
	}
	else if (region->type == 3)
	{
#if 1
		draw_key_button(14, c, screen);
		draw_arrow(2, 0, screen + 1);

#else
		for (j = 0; j<12; j++)
		{
			for (k = 0; k<12; k++)
			{
				if (key_arrow[k][j])
					screen[k] = c;
			}
			screen += DRAWSCREENWIDTH;
		}
#endif
	}
	else if (region->type == 4)
	{
#if 1
		draw_key_button(14, c, screen);
		draw_arrow(1, 0, screen + 1);

#else
		for (j = 0; j<12; j++)
		{
			for (k = 0; k<12; k++)
			{
				if (key_arrow[11 - j][11 - k])
					screen[k] = c;
			}
			screen += DRAWSCREENWIDTH;
		}
#endif
	}
	else if (region->type == 5)
	{
#if 1
		draw_key_button(14, c, screen);
		draw_arrow(3, 0, screen + 1);

#else
		for (j = 0; j < 12; j++)
		{
			for (k = 0; k < 12; k++)
			{
				if (key_arrow[11 - k][j])
					screen[k] = c;
			}
			screen += DRAWSCREENWIDTH;
		}
#endif
	}
	else if (region->type == 6)
	{
		//left/right sides
		vline(x, y + 1, 14, c);
		vline(x + 14, y + 1, 14, c);
		//top/bottom
		hline(x + 1, y + 1, 14, c);
		hline(x + 1, y + 14, 14, c);
	}
}

void keyboard_draw(int force)
{
	//return;
#ifdef ARM9
	int i, h;
	sregion_t *region;
	int count;
	u16 width, height;

	//keyboard_visible = 1;
	extern boolean FileMenuKeySteal;

	if (keyboard_visible == 0 && !FileMenuKeySteal)
	{
		return;
	}
	keyboard_resized = 0;

	keyboard_screen = subscreen;// ds_bottom_screen;
	keyboard_screen += ((320 - 256) / 2);
	keyboard_screen += (320 * ((200 - 192) / 2));

	//keyboard_screen = (u16*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &width, &height);

	//see if the keyboard layout has changed or automap toggled
	if (force ||
		keyboard_visible != keyboard_visible_last || 
		(automapactive ^ automapactive_last) ||
		automapactive) {
		keyboard_vofs = KEYBOARD_FULL_VOFS;
		keyboard_hofs = KEYBOARD_HOFS;
		h = keyboard_visible == 1 ? 19 : 24;

		if (keyboard_visible == 2) {
			keyboard_vofs = KEYBOARD_MINI_VOFS;
		}
		//clear the console and set window size
		//if (!automapactive) {
		//	memset(subscreen, 0, DRAWSCREENWIDTH * DRAWSCREENHEIGHT);
		//}

		//printf("full refresh: %d %d\n", keyboard_visible, keyboard_visible_last);
		keyboard_visible_last = keyboard_visible;
		automapactive_last = automapactive;
	}
	else {
		//the keyboard layout has not changed so no need to draw everything
		return;
	}

	region = key_array;
	count = key_array_count;
	if (keyboard_visible == 2)
	{
		region = key_button_array;
		count = key_button_array_count;
	}



	for (i = 0; i<count; i++) {
		keyboard_draw_region(&region[i], -1, automapactive ? keyboard_fg_am : keyboard_fg);
	}

	//draw down keys on full draw
	keyboard_draw_region(last_touching, last_index, automapactive ? keyboard_fg_am : keyboard_fg);
	keyboard_draw_region(key_touching, key_touching_index, automapactive ? keyboard_bg_am : keyboard_bg);
	UpdateState |= I_FULLSCRN;
#endif
}

