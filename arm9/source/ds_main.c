//#include <nds/registers_alt.h>
#include <stdlib.h>
#include <stdio.h>
#include <nds.h>
#include <nds/arm9/console.h>

#ifdef _HEXENDS_GBFS
	#include <gbfs.h>
#else
	#include <fat.h>
#endif

#include "ds_defs.h"

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

#define consolePrintf iprintf

extern byte softPCScreenPal[256*3];
extern byte softPCScreenBuffer[H2SCREENWIDTH*H2SCREENHEIGHT];
extern byte softSubPCScreenBuffer[H2DRAWSCREENWIDTH*H2DRAWSCREENHEIGHT];

extern void ibm_main(int argc, char **argv);
extern void ibm_updatefromdskeys(int dsKeys);
extern void *Z_Malloc (int size, int tag, void *ptr);
extern void Z_Free (void *ptr);
#define	PU_STATIC		1

#ifdef _HEXENDS_GBFS
const GBFS_FILE *g_gbfsBegin = 0;
byte *g_hexenWad = 0;
#endif

//malloc wrapper
void *ds_malloc(int size)
{
	//return Z_Malloc(size, PU_STATIC, 0);
	return malloc(size);
}

//free wrapper
void ds_free(void *ptr)
{
	//Z_Free(ptr);
	return free(ptr);
}

#ifdef _HEXENDS_GBFS
//init gbfs stuff
void DS_InitDSFileSystem(void)
{
	//clear bit 7 of WAIT_CR register so arm9 may access gba cart rom
	WAIT_CR &= ~(1<<7);

	g_gbfsBegin = find_first_gbfs_file((void *)0x8000000);
	if (!g_gbfsBegin)
	{
		return;
	}
}

//filesystem utility function
byte *DS_GetROMOffset(const char *filename)
{
	if (!g_gbfsBegin)
	{
		return 0;
	}

	return (byte *)gbfs_get_obj(g_gbfsBegin, filename, NULL);
}
#endif

//cap rgb util func
int CapRGB(int x)
{
	if (x < 0)
	{
		return 0;
	}
	if (x > 31)
	{
		return 31;
	}

	return x;
}

//update hardware palette
void DS_UpdatePalette(void)
{
	float scale = 8.0f;
	int i = 0;

	while (i < 256)
	{
		int irgb[3];
		irgb[0] = CapRGB((int)(softPCScreenPal[i*3]/scale));
		irgb[1] = CapRGB((int)(softPCScreenPal[(i*3)+1]/scale));
		irgb[2] = CapRGB((int)(softPCScreenPal[(i*3)+2]/scale));

		BG_PALETTE[i] = ((irgb[0])|(irgb[1]<<5)|(irgb[2]<<10)|(1<<15));
#ifndef _DS_SUBDEBUG
		BG_PALETTE_SUB[i] = ((irgb[0])|(irgb[1]<<5)|(irgb[2]<<10)|(1<<15));
#endif
		i++;
	}
}

//get game tics
int DS_GetH2Tics(void) {
//	int i = ((TIMER1_DATA*(1<<16))+TIMER0_DATA)/32;
//	float f = (float)i;
//	return (int)(f*0.035f);
	//not exact but faster.
	return ((TIMER1_DATA*(1<<16))+TIMER0_DATA)/914;
}

#ifdef _DS_PRINT_TIMINGS
int DS_GetPrecisionTime(void) {
	return ((TIMER1_DATA*(1<<16))+TIMER0_DATA);
}
#endif

//push software buffer to the screen
void DS_SoftToScreen(int subchanged)
{
#ifdef _DS_RESOLUTION
	while(DMA_CR(0) & DMA_BUSY);
	dmaCopyWordsAsynch(0, (const void *)softPCScreenBuffer, (void *)BG_BMP_RAM(0), H2SCREENWIDTH*H2SCREENHEIGHT);
	
	#ifndef _DS_SUBDEBUG
	if (subchanged)
	{
		uint8 *out = (uint8 *)BG_BMP_RAM_SUB(0);
		uint8 *in = (uint8 *)softSubPCScreenBuffer;
		int h = 0;
		while (h < H2DRAWSCREENHEIGHT)
		{
			dmaCopyWords(1, in, out, H2DRAWSCREENWIDTH);
			out += 512;
			in += H2DRAWSCREENWIDTH;

			h++;
		}
	}
	#endif
#else
	uint8 *out = (uint8 *)BG_BMP_RAM(0);
	uint8 *in = (uint8 *)softPCScreenBuffer;
	int h = 0;
	while (h < H2SCREENHEIGHT)
	{
		dmaCopyWords(0, in, out, H2SCREENWIDTH);
		out += 512;
		in += H2SCREENWIDTH;

		h++;
	}
#endif
}


void on_irq() 
{	
  if(REG_IF & IRQ_VBLANK) {
	//DS_SoftToScreen();

    // Tell the DS we handled the VBLANK interrupt
    // 2010 VBLANK_INTR_WAIT_FLAGS |= IRQ_VBLANK;
    REG_IF |= IRQ_VBLANK;
  }
  else {
    // Ignore all other interrupts
    REG_IF = REG_IF;
  }
}

//switches the subscreen to console mode
void DS_SwitchToConsoleMode(void)
{
	//setup the sub display
	videoSetModeSub(MODE_0_2D|DISPLAY_BG0_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG);
	REG_BG0CNT_SUB = BG_MAP_BASE(31);
	BG_PALETTE_SUB[255] = RGB15(31,31,31);

	//init debug console
	//2010 consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);
	consoleDemoInit();

	consoleClear();
}

extern int detailLevel;
//begin user code
int main(int argc, char **argv)
{

	lcdMainOnTop();
	soundEnable(); //elhobbs
	
	// IRQ basic setup
	// 2010 irqInitHandler(on_irq);
	
	//setup the main display
	videoSetMode(MODE_5_2D|DISPLAY_BG3_ACTIVE);
	vramSetBankA(VRAM_A_MAIN_BG);
#ifdef _DS_RESOLUTION
	REG_BG3CNT = BG_BMP8_256x256|BG_BMP_BASE(0);
#else
	REG_BG3CNT = BG_BMP8_512x512|BG_BMP_BASE(0);
#endif

#ifdef _DS_SUBDEBUG
	//setup the sub display
	videoSetModeSub(MODE_0_2D|DISPLAY_BG0_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG);
	REG_BG0CNT_SUB = BG_MAP_BASE(31);
	BG_PALETTE_SUB[255] = RGB15(31,31,31);

	//init debug console
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);
#else
	videoSetModeSub(MODE_5_2D|DISPLAY_BG3_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG);
	REG_BG3CNT_SUB = BG_BMP8_512x512|BG_BMP_BASE(0);

	REG_BG3PB_SUB = 0;
	REG_BG3PC_SUB = 0;
	REG_BG3PA_SUB = 256+64;
	REG_BG3PD_SUB = 256+12;
#endif

	//enable timers for keeping track of a normal time value every frame.
	TIMER0_CR = TIMER_ENABLE|TIMER_DIV_1024;
	TIMER1_CR = TIMER_ENABLE|TIMER_CASCADE;

	detailLevel = 0;//1;

	//set background scaling
	REG_BG3PB = 0;
	REG_BG3PC = 0;
#ifdef _DS_RESOLUTION
	REG_BG3PA = 256;
	REG_BG3PD = 256;
#else
	REG_BG3PA = 256+64;
	BG3_YDY = 256;
#endif
	
	#ifdef _HEXENDS_GBFS
		//init gbfs
		DS_InitDSFileSystem();
	#else
		fatInitDefault();
	#endif

	//hop into the old hexen loop logic
	ibm_main(argc, argv);

	//just in case
	consolePrintf("Fatal error: Broke out of main loop.");
	while (1)
	{
		swiWaitForVBlank();
	}
}
