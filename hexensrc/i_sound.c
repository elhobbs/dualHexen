
// I_SOUND.C

#include <stdio.h>
#include "h2def.h"
//#include "dmx.h"
#include "sounds.h"
#include "i_sound.h"

//rww begin
#ifdef _HEXENDS
#include <nds.h>
//#include "../source/ds_shared.h"
#endif

extern sfxinfo_t S_sfx[];
extern void *W_RetagSoundLump(int lump, int tag);
//rww end

/*
===============
=
= I_StartupTimer
=
===============
*/

int tsm_ID = -1;

void I_StartupTimer (void)
{
	//rww begin
	/*
#ifndef NOTIMER
	extern int I_TimerISR(void);

	ST_Message("  I_StartupTimer()\n");
	// installs master timer.  Must be done before StartupTimer()!
	TSM_Install(SND_TICRATE);
	tsm_ID = TSM_NewService (I_TimerISR, 35, 255, 0); // max priority
	if (tsm_ID == -1)
	{
		I_Error("Can't register 35 Hz timer w/ DMX library");
	}
#endif
	*/
	//rww end
}

void I_ShutdownTimer (void)
{
	//rww begin
	/*
	TSM_DelService(tsm_ID);
	TSM_Remove();
	*/
	//rww end
}

/*
 *
 *                           SOUND HEADER & DATA
 *
 *
 */

// sound information
#if 0
const char *dnames[] = {"None",
			"PC_Speaker",
			"Adlib",
			"Sound_Blaster",
			"ProAudio_Spectrum16",
			"Gravis_Ultrasound",
			"MPU",
			"AWE32"
			};
#endif

const char snd_prefixen[] = { 'P', 'P', 'A', 'S', 'S', 'S', 'M',
  'M', 'M', 'S' };

int snd_Channels;
int snd_DesiredMusicDevice, snd_DesiredSfxDevice;
int snd_MusicDevice,    // current music card # (index to dmxCodes)
	snd_SfxDevice,      // current sfx card # (index to dmxCodes)
	snd_MaxVolume,      // maximum volume for sound
	snd_MusicVolume;    // maximum volume for music
int dmxCodes[NUM_SCARDS]; // the dmx code for a given card

int     snd_SBport, snd_SBirq, snd_SBdma;       // sound blaster variables
int     snd_Mport;                              // midi variables

extern boolean  snd_MusicAvail, // whether music is available
		snd_SfxAvail;   // whether sfx are available

void I_PauseSong(int handle)
{
	//rww begin
  //MUS_PauseSong(handle);
	//rww end
}

void I_ResumeSong(int handle)
{
	//rww begin
  //MUS_ResumeSong(handle);
	//rww end
}

void I_SetMusicVolume(int volume)
{
	//rww begin
  //MUS_SetMasterVolume(volume*8);
	//rww end
//  snd_MusicVolume = volume;
}

void I_SetSfxVolume(int volume)
{
  snd_MaxVolume = volume; // THROW AWAY?
}

/*
 *
 *                              SONG API
 *
 */

int I_RegisterSong(void *data)
{
	//rww begin
	/*
  int rc = MUS_RegisterSong(data);
#ifdef SNDDEBUG
  if (rc<0) ST_Message("    MUS_Reg() returned %d\n", rc);
#endif
  return rc;
  */
	return 0;
	//rww end
}

void I_UnRegisterSong(int handle)
{
	//rww begin
	/*
  int rc = MUS_UnregisterSong(handle);
#ifdef SNDDEBUG
  if (rc < 0) ST_Message("    MUS_Unreg() returned %d\n", rc);
#endif
  */
	//rww end
}

int I_QrySongPlaying(int handle)
{
	//rww begin
	/*
  int rc = MUS_QrySongPlaying(handle);
#ifdef SNDDEBUG
  if (rc < 0) ST_Message("    MUS_QrySP() returned %d\n", rc);
#endif
  return rc;
  */
	return 0;
	//rww end
}

// Stops a song.  MUST be called before I_UnregisterSong().

void I_StopSong(int handle)
{
	//rww begin
	/*
  int rc;
  rc = MUS_StopSong(handle);
#ifdef SNDDEBUG
  if (rc < 0) ST_Message("    MUS_StopSong() returned %d\n", rc);
#endif
  */
	//rww end
/*
  // Fucking kluge pause
  {
	int s;
	extern volatile int ticcount;
	for (s=ticcount ; ticcount - s < 10 ; );
  }
*/
}

void I_PlaySong(int handle, boolean looping)
{
	//rww begin
	/*
  int rc;
  rc = MUS_ChainSong(handle, looping ? handle : -1);
#ifdef SNDDEBUG
  if (rc < 0) ST_Message("    MUS_ChainSong() returned %d\n", rc);
#endif
  rc = MUS_PlaySong(handle, snd_MusicVolume);
#ifdef SNDDEBUG
  if (rc < 0) ST_Message("    MUS_PlaySong() returned %d\n", rc);
#endif
  */
	//rww end

}

/*
 *
 *                                 SOUND FX API
 *
 */

// Gets lump nums of the named sound.  Returns pointer which will be
// passed to I_StartSound() when you want to start an SFX.  Must be
// sure to pass this to UngetSoundEffect() so that they can be
// freed!


int I_GetSfxLumpNum(sfxinfo_t *sound)
{
  return W_GetNumForName(sound->lumpname);

}

//rww begin
extern void *W_CacheLumpNumSound(int lump, int tag);
void I_SetSfxTagDead(sfxinfo_t *sound)
{
	//ST_Message("sound %s dead  \n", sound->lumpname);
	sound->snd_ptr = W_RetagSoundLump(sound->lumpnum,
						PU_CACHE);
}

void I_SetSoundTags(void)
{
#ifdef _HEXENDS
	int i = 0;
	
	/*
	if (g_sharedSound->clearSoundNum > 0)
	{
		ST_Message("clearing %i sounds \n", g_sharedSound->clearSoundNum);
	}
	*/
	
	
	//2010
	/*
	while (i < g_sharedSound->clearSoundNum)
	{
		if (g_sharedSound->clearSoundIdImportance[i] != -1)
		{
	        I_SetSfxTagDead(&S_sfx[g_sharedSound->clearSoundIdImportance[i]]);
		}
		i++;
	}
	g_sharedSound->clearSoundNum = 0;
	*/
#endif
}
//rww end

int I_StartSound (int id, void *data, int vol, int sep, int pitch, int priority)
{
	//rww begin
#ifdef _HEXENDS
	//2010 pTransferSoundData soundData;
#endif
	sfxinfo_t *sfx;
	int len;

	if (!data)
	{
		return 0;
	}

	priority = priority/10;
	if (priority > 127)
	{
		priority = 127;
	}
	if (vol > 127)
	{
		vol = 127;
	}
	sfx = &S_sfx[id];
	len = W_LumpLength(sfx->lumpnum);



#ifdef _HEXENDS
      soundPlaySample((const void *)data,
      SoundFormat_8Bit,
      len,
      11025,
      vol,
      63,
      0,
      0); 

	/*
	IPC->soundData = (pTransferSound)(0x027FF000+sizeof(TransferRegion)+sizeof(sharedSoundData_t));
	if (IPC->soundData->count >= 16)
	{
		return 0;
	}

	len = (len-SOUND_DATA_OFFSET)>>2;
	if (len <= 0)
	{
		return 0;
	}

	soundData = &IPC->soundData->data[IPC->soundData->count];
	soundData->data = (const void *)((u32)(data)+SOUND_DATA_OFFSET);
	soundData->len = len;
	soundData->rate = id; //11025; //replaced rate with sound_id
	soundData->vol = (u8)vol;
	soundData->pan = 63;//(u8)sep; //FIXME - convert sep to ds-friendly pan value
	soundData->format = priority; //replaced 8bit with prior data
	IPC->soundData->count++;

	//ST_Message("sound: %s, %i, %i, %i      \n", sfx->lumpname, len, id, sfx->lumpnum);
	*/
#else
	//no sound in win32, set dead immediately
    I_SetSfxTagDead(sfx);
#endif
	//return SFX_PlayPatch(data, pitch, sep, vol, 0, 0);
	return 0;
	//rww end
}

void I_StopSound(int handle)
{
	//rww begin FIXME
//  extern volatile long gDmaCount;
//  long waittocount;
  //SFX_StopPatch(handle);
	//rww end
//  waittocount = gDmaCount + 2;
//  while (gDmaCount < waittocount) ;
}

int I_SoundIsPlaying(int handle)
{
	//rww begin FIXME
  //return SFX_Playing(handle);
	return 0;
	//rww end
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
	//rww begin FIXME
  //SFX_SetOrigin(handle, pitch, sep, vol);
	//rww end
}

/*
 *
 *                                                      SOUND STARTUP STUFF
 *
 *
 */

//
// Why PC's Suck, Reason #8712
//

//rww begin
/*
void I_sndArbitrateCards(void)
{
	char tmp[160];
  boolean gus, adlib, pc, sb, midi;
  int i, rc, mputype, p, opltype, wait, dmxlump;

  snd_MusicDevice = snd_DesiredMusicDevice;
  snd_SfxDevice = snd_DesiredSfxDevice;

  // check command-line parameters- overrides config file
  //
  if (M_CheckParm("-nosound")) snd_MusicDevice = snd_SfxDevice = snd_none;
  if (M_CheckParm("-nosfx")) snd_SfxDevice = snd_none;
  if (M_CheckParm("-nomusic")) snd_MusicDevice = snd_none;

  if (snd_MusicDevice > snd_MPU && snd_MusicDevice <= snd_MPU3)
	snd_MusicDevice = snd_MPU;
  if (snd_MusicDevice == snd_SB)
	snd_MusicDevice = snd_Adlib;
  if (snd_MusicDevice == snd_PAS)
	snd_MusicDevice = snd_Adlib;

  // figure out what i've got to initialize
  //
  gus = snd_MusicDevice == snd_GUS || snd_SfxDevice == snd_GUS;
  sb = snd_SfxDevice == snd_SB || snd_MusicDevice == snd_SB;
  adlib = snd_MusicDevice == snd_Adlib ;
  pc = snd_SfxDevice == snd_PC;
  midi = snd_MusicDevice == snd_MPU;

  // initialize whatever i've got
  //
  if (gus)
  {
	if (GF1_Detect()) ST_Message("    Dude.  The GUS ain't responding.\n");
	else
	{
	  dmxlump = W_GetNumForName("dmxgus");
	  GF1_SetMap(W_CacheLumpNum(dmxlump, PU_CACHE), lumpinfo[dmxlump].size);
	}

  }
  if (sb)
  {
	if(debugmode)
	{
	  ST_Message("  Sound cfg p=0x%x, i=%d, d=%d\n",
	  	snd_SBport, snd_SBirq, snd_SBdma);
	}
	if (SB_Detect(&snd_SBport, &snd_SBirq, &snd_SBdma, 0))
	{
	  ST_Message("    SB isn't responding at p=0x%x, i=%d, d=%d\n",
	  	snd_SBport, snd_SBirq, snd_SBdma);
	}
	else SB_SetCard(snd_SBport, snd_SBirq, snd_SBdma);

	if(debugmode)
	{
	  ST_Message("    SB_Detect returned p=0x%x, i=%d, d=%d\n",
	  	snd_SBport, snd_SBirq, snd_SBdma);
	}
  }

  if (adlib)
  {
	if (AL_Detect(&wait,0))
	{
	  	ST_Message("    Dude.  The Adlib isn't responding.\n");
	}
	else
	{
		AL_SetCard(wait, W_CacheLumpName("genmidi", PU_STATIC));
	}
  }

  if (midi)
  {
	if (debugmode)
	{
		ST_Message("    cfg p=0x%x\n", snd_Mport);
	}

	if (MPU_Detect(&snd_Mport, &i))
	{
	  ST_Message("    The MPU-401 isn't reponding @ p=0x%x.\n", snd_Mport);
	}
	else MPU_SetCard(snd_Mport);
  }

}
*/
//rww end

// inits all sound stuff

void I_StartupSound (void)
{
	//rww begin
	/*
  int rc, i;

  if (debugmode)
	ST_Message("I_StartupSound: Hope you hear a pop.\n");

  // initialize dmxCodes[]
  dmxCodes[0] = 0;
  dmxCodes[snd_PC] = AHW_PC_SPEAKER;
  dmxCodes[snd_Adlib] = AHW_ADLIB;
  dmxCodes[snd_SB] = AHW_SOUND_BLASTER;
  dmxCodes[snd_PAS] = AHW_MEDIA_VISION;
  dmxCodes[snd_GUS] = AHW_ULTRA_SOUND;
  dmxCodes[snd_MPU] = AHW_MPU_401;
  dmxCodes[snd_MPU2] = AHW_MPU_401;
  dmxCodes[snd_MPU3] = AHW_MPU_401;
  dmxCodes[snd_AWE] = AHW_AWE32;
  dmxCodes[snd_CDMUSIC] = 0;

  // inits sound library timer stuff
  I_StartupTimer();

  // pick the sound cards i'm going to use
  //
  I_sndArbitrateCards();

  if (debugmode)
  {
	ST_Message("    Music device #%d & dmxCode=%d,", snd_MusicDevice,
	  dmxCodes[snd_MusicDevice]);
	ST_Message(" Sfx device #%d & dmxCode=%d\n", snd_SfxDevice,
	  dmxCodes[snd_SfxDevice]);
  }

  // inits DMX sound library
  ST_Message("    Calling DMX_Init...");
  rc = DMX_Init(SND_TICRATE, SND_MAXSONGS, dmxCodes[snd_MusicDevice],
	dmxCodes[snd_SfxDevice]);

  if (debugmode)
  {
	ST_Message(" DMX_Init() returned %d\n", rc);
  }

  */
	//rww end
}

// shuts down all sound stuff

void I_ShutdownSound (void)
{
	//rww begin
	/*
  DMX_DeInit();
  I_ShutdownTimer();
  */
	//rww end
}

void I_SetChannels(int channels)
{
	//rww begin
	/*
  WAV_PlayMode(channels, SND_SAMPLERATE);
  */
	//rww end
}
