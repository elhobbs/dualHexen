
//**************************************************************************
//**
//** sounds.c : Heretic 2 : Raven Software, Corp.
//**
//** $RCSfile: sounds.c,v $
//** $Revision: 1.83 $
//** $Date: 96/01/06 18:37:44 $
//** $Author: bgokey $
//**
//**************************************************************************

#include "h2def.h"
#include "sounds.h"

// Music info

/*
musicinfo_t S_music[] =
{
	{ "MUS_E1M1", 0 }, // 1-1
	{ "MUS_E1M2", 0 },
	{ "MUS_E1M3", 0 },
	{ "MUS_E1M4", 0 },
	{ "MUS_E1M5", 0 },
	{ "MUS_E1M6", 0 },
	{ "MUS_E1M7", 0 },
	{ "MUS_E1M8", 0 },
	{ "MUS_E1M9", 0 },
	{ "MUS_E2M1", 0 }, // 2-1
	{ "MUS_E2M2", 0 },
	{ "MUS_E2M3", 0 },
	{ "MUS_E2M4", 0 },
	{ "MUS_E1M4", 0 },
	{ "MUS_E2M6", 0 },
	{ "MUS_E2M7", 0 },
	{ "MUS_E2M8", 0 },
	{ "MUS_E2M9", 0 },
	{ "MUS_E1M1", 0 }, // 3-1
	{ "MUS_E3M2", 0 },
	{ "MUS_E3M3", 0 },
	{ "MUS_E1M6", 0 },
	{ "MUS_E1M3", 0 },
	{ "MUS_E1M2", 0 },
	{ "MUS_E1M5", 0 },
	{ "MUS_E1M9", 0 },
	{ "MUS_E2M6", 0 },
	{ "MUS_E1M6", 0 }, // 4-1
	{ "MUS_TITL", 0 },
	{ "MUS_INTR", 0 },
	{ "MUS_CPTD", 0 }
};
*/

// Sound info

sfxinfo_t S_sfx[] =
{
	// tagname, lumpname, priority, usefulness, snd_ptr, lumpnum, numchannels,
	//		pitchshift
	//rww begin - had to fill in lump names
	{ "", "", 0, -1, NULL, 0, 0, 0 },
	{ "PlayerFighterNormalDeath", "fgtddth", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerFighterCrazyDeath", "fgtcdth", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerFighterExtreme1Death", "fgtxdth1", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerFighterExtreme2Death", "fgtxdth2", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerFighterExtreme3Death", "fgtxdth3", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerFighterBurnDeath", "plrburn", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericNormalDeath", "plrdth", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericCrazyDeath", "plrcdth", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericExtreme1Death", "clxdth1", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericExtreme2Death", "clxdth2", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericExtreme3Death", "clxdth3", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericBurnDeath", "plrburn", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerMageNormalDeath", "mgdth", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerMageCrazyDeath", "mgcdth", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerMageExtreme1Death", "mgxdth1", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerMageExtreme2Death", "mgxdth2", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerMageExtreme3Death", "mgxdth3", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerMageBurnDeath", "plrburn", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerFighterPain", "fgtpain", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericPain", "plrpain3", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerMagePain", "mgpain", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerFighterGrunt", "fgtgrunt", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericGrunt", "grunt1", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerMageGrunt", "mggrunt", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerLand", "feet4", 32, -1, NULL, 0, 2, 1 },
	{ "PlayerPoisonCough", "pois2", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerFighterFallingScream", "fgtfall", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerClericFallingScream", "fall1", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerMageFallingScream", "mgfall", 256, -1, NULL, 0, 2, 0 },
	{ "PlayerFallingSplat", "fallhit", 256, -1, NULL, 0, 2, 1 },
	{ "PlayerFighterFailedUse", "fgtgrunt", 256, -1, NULL, 0, 1, 1 },
	{ "PlayerClericFailedUse", "grunt1", 256, -1, NULL, 0, 1, 1 },
	{ "PlayerMageFailedUse", "mggrunt", 256, -1, NULL, 0, 1, 0 },
	{ "PlatformStart", "dorstp3", 36, -1, NULL, 0, 2, 1 },
	{ "PlatformStartMetal", "mtlstrt1", 36, -1, NULL, 0, 2, 1 },
	{ "PlatformStop", "dorstp1b", 40, -1, NULL, 0, 2, 1 },
	{ "StoneMove", "stnmv4", 32, -1, NULL, 0, 2, 1 },
	{ "MetalMove", "mtlmv3", 32, -1, NULL, 0, 2, 1 },
	{ "DoorOpen", "drltch3a", 36, -1, NULL, 0, 2, 1 },
	{ "DoorLocked", "locked", 36, -1, NULL, 0, 2, 1 },
	{ "DoorOpenMetal", "stmdr1", 36, -1, NULL, 0, 2, 1 },
	{ "DoorCloseMetal", "drstp2", 36, -1, NULL, 0, 2, 1 },
	{ "DoorCloseLight", "dorstp4", 36, -1, NULL, 0, 2, 1 },
	{ "DoorCloseHeavy", "drstp7", 36, -1, NULL, 0, 2, 1 },
	{ "DoorCreak", "drcrk5a", 36, -1, NULL, 0, 2, 1 },
	{ "PickupWeapon", "sample11", 36, -1, NULL, 0, 2, 0 },
	{ "PickupArtifact", "art1", 36, -1, NULL, 0, 2, 1 },
	{ "PickupKey", "keys2a", 36, -1, NULL, 0, 2, 1 },
	{ "PickupItem", "picup5", 36, -1, NULL, 0, 2, 1 },
	{ "PickupPiece", "piece1", 36, -1, NULL, 0, 2, 0 },
	{ "WeaponBuild", "sample11", 36, -1, NULL, 0, 2, 0 },
	{ "UseArtifact", "artact1", 36, -1, NULL, 0, 2, 1 },
	{ "BlastRadius", "blastr", 36, -1, NULL, 0, 2, 1 },
	{ "Teleport", "tport1", 256, -1, NULL, 0, 2, 1 },
	{ "ThunderCrash", "thndr1", 30, -1, NULL, 0, 2, 1 },
	{ "FighterPunchMiss", "punchmis", 80, -1, NULL, 0, 2, 1 },
	{ "FighterPunchHitThing", "mumpun5", 80, -1, NULL, 0, 2, 1 },
	{ "FighterPunchHitWall", "punch8", 80, -1, NULL, 0, 2, 1 },
	{ "FighterGrunt", "gntmis1", 80, -1, NULL, 0, 2, 1 },
	{ "FighterAxeHitThing", "axe5", 80, -1, NULL, 0, 2, 1 },
	{ "FighterHammerMiss", "hammis2", 80, -1, NULL, 0, 2, 1 },
	{ "FighterHammerHitThing", "hmhit1a", 80, -1, NULL, 0, 2, 1 },
	{ "FighterHammerHitWall", "hamwal1", 80, -1, NULL, 0, 2, 1 },
	{ "FighterHammerContinuous", "hamfly1", 32, -1, NULL, 0, 2, 1 },
	{ "FighterHammerExplode", "impact3", 80, -1, NULL, 0, 2, 1 },
	{ "FighterSwordFire", "sword2", 80, -1, NULL, 0, 2, 1 },
	{ "FighterSwordExplode", "impact3", 80, -1, NULL, 0, 2, 1 },
	{ "ClericCStaffFire", "spell1", 80, -1, NULL, 0, 2, 1 },
	{ "ClericCStaffExplode", "glbhit4", 40, -1, NULL, 0, 2, 1 },
	{ "ClericCStaffHitThing", "vamp5", 80, -1, NULL, 0, 2, 1 },
	{ "ClericFlameFire", "strike3", 80, -1, NULL, 0, 2, 1 },
	{ "ClericFlameExplode", "strike1", 80, -1, NULL, 0, 2, 1 },
	{ "ClericFlameCircle", "firedhit", 80, -1, NULL, 0, 2, 1 },
	{ "MageWandFire", "wand4", 80, -1, NULL, 0, 2, 1 },
	{ "MageLightningFire", "lite2", 80, -1, NULL, 0, 2, 1 },
	{ "MageLightningZap", "gntact1", 32, -1, NULL, 0, 2, 1 },
	{ "MageLightningContinuous", "gntpow", 32, -1, NULL, 0, 2, 1 },
	{ "MageLightningReady", "wepele2", 30, -1, NULL, 0, 2, 1 },
	{ "MageShardsFire","cone3", 80, -1, NULL, 0, 2, 1 },
	{ "MageShardsExplode","shards1b", 36, -1, NULL, 0, 2, 1 },
	{ "MageStaffFire","mage4", 80, -1, NULL, 0, 2, 1 },
	{ "MageStaffExplode","mageball", 40, -1, NULL, 0, 2, 1 },
	{ "Switch1", "chnswch8", 32, -1, NULL, 0, 2, 1 },
	{ "Switch2", "swtch6", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentSight", "wtrcrt7", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentActive", "srfc3", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentPain", "serppn1", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentAttack", "wtrswip", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentMeleeHit", "wtrhit", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentDeath", "srpdth1", 40, -1, NULL, 0, 2, 1 },
	{ "SerpentBirth", "srfc1", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentFXContinuous", "glbhiss1", 32, -1, NULL, 0, 2, 1 },
	{ "SerpentFXHit", "glbhit4", 32, -1, NULL, 0, 2, 1 },
	{ "PotteryExplode", "potbrk1", 32, -1, NULL, 0, 2, 1 },
	{ "Drip", "blddrp1", 32, -1, NULL, 0, 2, 1 },
	{ "CentaurSight", "taur1", 32, -1, NULL, 0, 2, 1 },
	{ "CentaurActive", "taur2", 32, -1, NULL, 0, 2, 1 },
	{ "CentaurPain", "taur4", 32, -1, NULL, 0, 2, 1 },
	{ "CentaurAttack", "centhit2", 32, -1, NULL, 0, 2, 1 },
	{ "CentaurDeath", "cntdth1", 40, -1, NULL, 0, 2, 1 },
	{ "CentaurLeaderAttack", "cntshld4", 32, -1, NULL, 0, 2, 1 },
	{ "CentaurMissileExplode", "impact3", 32, -1, NULL, 0, 2, 1 },
	{ "Wind", "wind3", 1, -1, NULL, 0, 2, 1 },
	{ "BishopSight", "syab2d", 32, -1, NULL, 0, 2, 1 },
	{ "BishopActive", "stb1d", 32, -1, NULL, 0, 2, 1 },
	{ "BishopPain", "bshpn1", 32, -1, NULL, 0, 2, 1 },
	{ "BishopAttack", "pop", 32, -1, NULL, 0, 2, 1 },
	{ "BishopDeath", "bishdth1", 40, -1, NULL, 0, 2, 1 },
	{ "BishopMissileExplode", "bshhit2", 32, -1, NULL, 0, 2, 1 },
	{ "BishopBlur", "blur6", 32, -1, NULL, 0, 2, 1 },
	{ "DemonSight", "sbtsit5", 32, -1, NULL, 0, 2, 1 },
	{ "DemonActive", "sbtsit5", 32, -1, NULL, 0, 2, 1 },
	{ "DemonPain", "minact1", 32, -1, NULL, 0, 2, 1 },
	{ "DemonAttack", "dematk2", 32, -1, NULL, 0, 2, 1 },
	{ "DemonMissileFire", "impfire2", 32, -1, NULL, 0, 2, 1 },
	{ "DemonMissileExplode", "impact3", 32, -1, NULL, 0, 2, 1 },
	{ "DemonDeath", "srpdth1", 40, -1, NULL, 0, 2, 1 },
	{ "WraithSight", "raith5a", 32, -1, NULL, 0, 2, 1 },
	{ "WraithActive", "raith3", 32, -1, NULL, 0, 2, 1 },
	{ "WraithPain", "raith4a", 32, -1, NULL, 0, 2, 1 },
	{ "WraithAttack", "raith1b", 32, -1, NULL, 0, 2, 1 },
	{ "WraithMissileFire", "impfire2", 32, -1, NULL, 0, 2, 1 },
	{ "WraithMissileExplode", "impact3", 32, -1, NULL, 0, 2, 1 },
	{ "WraithDeath", "rathdth2", 40, -1, NULL, 0, 2, 1 },
	{ "PigActive1", "pigrunt1", 32, -1, NULL, 0, 2, 1 },
	{ "PigActive2", "squeal1", 32, -1, NULL, 0, 2, 1 },
	{ "PigPain", "pigpain2", 32, -1, NULL, 0, 2, 1 },
	{ "PigAttack", "bite4", 32, -1, NULL, 0, 2, 1 },
	{ "PigDeath", "pigdth2", 40, -1, NULL, 0, 2, 1 },
	{ "MaulatorSight", "minsit1", 32, -1, NULL, 0, 2, 1 },
	{ "MaulatorActive", "minact2", 32, -1, NULL, 0, 2, 1 },
	{ "MaulatorPain", "minpain4", 32, -1, NULL, 0, 2, 1 },
	{ "MaulatorHamSwing", "hamblo8a", 32, -1, NULL, 0, 2, 1 },
	{ "MaulatorHamHit", "hamfir1", 32, -1, NULL, 0, 2, 1 },
	{ "MaulatorMissileHit", "impact3", 32, -1, NULL, 0, 2, 1 },
	{ "MaulatorDeath", "mindth4", 40, -1, NULL, 0, 2, 1 },
	{ "FreezeDeath", "icedth1", 40, -1, NULL, 0, 2, 1 },
	{ "FreezeShatter", "icebrk1a", 40, -1, NULL, 0, 2, 1 },
	{ "EttinSight", "cent2", 32, -1, NULL, 0, 2, 1 },
	{ "EttinActive", "cent2", 32, -1, NULL, 0, 2, 1 },
	{ "EttinPain", "cent1", 32, -1, NULL, 0, 2, 1 },
	{ "EttinAttack", "ethit1", 32, -1, NULL, 0, 2, 1 },
	{ "EttinDeath", "cntdth1", 40, -1, NULL, 0, 2, 1 },
	{ "FireDemonSpawn", "spawn3", 32, -1, NULL, 0, 2, 1 },
	{ "FireDemonActive", "fired5", 32, -1, NULL, 0, 2, 1 },
	{ "FireDemonPain", "fired2", 32, -1, NULL, 0, 2, 1 },
	{ "FireDemonAttack", "spit6", 32, -1, NULL, 0, 2, 1 },
	{ "FireDemonMissileHit", "firedhit", 32, -1, NULL, 0, 2, 1 },
	{ "FireDemonDeath", "fired3", 40, -1, NULL, 0, 2, 1 },
	{ "IceGuySight", "frosty1", 32, -1, NULL, 0, 2, 1 },
	{ "IceGuyActive", "frosty1", 32, -1, NULL, 0, 2, 1 },
	{ "IceGuyAttack", "frosty2", 32, -1, NULL, 0, 2, 1 },
	{ "IceGuyMissileExplode", "shards1b", 32, -1, NULL, 0, 2, 1 },
	{ "SorcererSight", "sorboss1", 256, -1, NULL, 0, 2, 1 },
	{ "SorcererActive", "sorboss2", 256, -1, NULL, 0, 2, 1 },
	{ "SorcererPain", "sorboss3", 256, -1, NULL, 0, 2, 1 },
	{ "SorcererSpellCast", "woosh3", 256, -1, NULL, 0, 2, 1 },
	{ "SorcererBallWoosh", "balls1", 256, -1, NULL, 0, 4, 1 },
	{ "SorcererDeathScream", "sordie2", 256, -1, NULL, 0, 2, 1 },
	{ "SorcererBishopSpawn", "bishapp", 80, -1, NULL, 0, 2, 1 },
	{ "SorcererBallPop", "cork", 80, -1, NULL, 0, 2, 1 },
	{ "SorcererBallBounce", "bounce2", 80, -1, NULL, 0, 3, 1 },
	{ "SorcererBallExplode", "impact3", 80, -1, NULL, 0, 3, 1 },
	{ "SorcererBigBallExplode", "sorblexp", 80, -1, NULL, 0, 3, 1 },
	{ "SorcererHeadScream", "sorblexp", 256, -1, NULL, 0, 2, 1 },
	{ "DragonSight", "dragsit1", 64, -1, NULL, 0, 2, 1 },
	{ "DragonActive", "dragsit1", 64, -1, NULL, 0, 2, 1 },
	{ "DragonWingflap", "wing1", 64, -1, NULL, 0, 2, 1 },
	{ "DragonAttack", "mage4", 64, -1, NULL, 0, 2, 1 },
	{ "DragonPain", "dragpn2", 64, -1, NULL, 0, 2, 1 },
	{ "DragonDeath", "dragdie2", 64, -1, NULL, 0, 2, 1 },
	{ "DragonFireballExplode", "mageball", 32, -1, NULL, 0, 2, 1 },
	{ "KoraxSight", "korsit", 256, -1, NULL, 0, 2, 1 },
	{ "KoraxActive", "koract", 256, -1, NULL, 0, 2, 1 },
	{ "KoraxPain", "korpn1", 256, -1, NULL, 0, 2, 1 },
	{ "KoraxAttack", "koratk", 256, -1, NULL, 0, 2, 1 },
	{ "KoraxCommand", "korcom", 256, -1, NULL, 0, 2, 1 },
	{ "KoraxDeath", "kordth1", 256, -1, NULL, 0, 2, 1 },
	{ "KoraxStep", "step", 128, -1, NULL, 0, 2, 1 },
	{ "ThrustSpikeRaise", "spike3", 32, -1, NULL, 0, 2, 1 },
	{ "ThrustSpikeLower", "spike2", 32, -1, NULL, 0, 2, 1 },
	{ "GlassShatter", "glass5", 32, -1, NULL, 0, 2, 1 },
	{ "FlechetteBounce", "flech4", 32, -1, NULL, 0, 2, 1 },
	{ "FlechetteExplode", "firedhit", 32, -1, NULL, 0, 2, 1 },
	{ "LavaMove", "lava2", 36, -1, NULL, 0, 2, 1 },
	{ "WaterMove", "brook1", 36, -1, NULL, 0, 2, 1 },
	{ "IceStartMove", "icemv2", 36, -1, NULL, 0, 2, 1 },
	{ "EarthStartMove", "quake2", 36, -1, NULL, 0, 2, 1 },
	{ "WaterSplash", "gloop1", 32, -1, NULL, 0, 2, 1 },
	{ "LavaSizzle", "acid5", 32, -1, NULL, 0, 2, 1 },
	{ "SludgeGloop", "muck5", 32, -1, NULL, 0, 2, 1 },
	{ "HolySymbolFire", "holy3", 64, -1, NULL, 0, 2, 1 },
	{ "SpiritActive", "spirt7", 32, -1, NULL, 0, 2, 1 },
	{ "SpiritAttack", "spirt5", 32, -1, NULL, 0, 2, 1 },
	{ "SpiritDie", "spirts1", 32, -1, NULL, 0, 2, 1 },
	{ "ValveTurn", "swtchv4", 36, -1, NULL, 0, 2, 1 },
	{ "RopePull", "swtchrp1", 36, -1, NULL, 0, 2, 1 },
	{ "FlyBuzz", "fly3", 20, -1, NULL, 0, 2, 1 },
	{ "Ignite", "torch2", 32, -1, NULL, 0, 2, 1 },
	{ "PuzzleSuccess", "puzslv", 256, -1, NULL, 0, 2, 1 },
	{ "PuzzleFailFighter", "fgthmm", 256, -1, NULL, 0, 2, 1 },
	{ "PuzzleFailCleric", "clhmm", 256, -1, NULL, 0, 2, 1 },
	{ "PuzzleFailMage", "mghmm", 256, -1, NULL, 0, 2, 1 },
	{ "Earthquake", "quake2", 32, -1, NULL, 0, 2, 1 },
	{ "BellRing", "bellrng", 32, -1, NULL, 0, 2, 0 },
	{ "TreeBreak", "treebrk", 32, -1, NULL, 0, 2, 1 },
	{ "TreeExplode", "treeburn", 32, -1, NULL, 0, 2, 1 },
	{ "SuitofArmorBreak", "armorexp", 32, -1, NULL, 0, 2, 1 },
	{ "PoisonShroomPain", "stretch3", 20, -1, NULL, 0, 2, 1 },
	{ "PoisonShroomDeath", "puff1", 32, -1, NULL, 0, 2, 1 },
	{ "Ambient1", "insects1", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient2", "crkets", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient3", "crkets1", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient4", "katydid", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient5", "frogs", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient6", "owl", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient7", "bird", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient8", "shlurp", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient9", "bubble", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient10", "drop2", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient11", "rocks", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient12", "chains", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient13", "gong", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient14", "steel1", 1, -1, NULL, 0, 1, 1 },
	{ "Ambient15", "steel2", 1, -1, NULL, 0, 1, 1 },
	{ "StartupTick", "tick", 32, -1, NULL, 0, 2, 1 },
	{ "SwitchOtherLevel", "puzslv1", 32, -1, NULL, 0, 2, 1 },
	{ "Respawn", "respawn", 32, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceGreetings", "grtngs1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceReady", "ready1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceBlood", "blood1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceGame", "game1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceBoard", "board1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceWorship", "wrship1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceMaybe", "maybe1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceStrong", "strong1", 512, -1, NULL, 0, 2, 1 },
	{ "KoraxVoiceFace", "face1", 512, -1, NULL, 0, 2, 1 },
	{ "BatScream", "bats", 32, -1, NULL, 0, 2, 1 },
	{ "Chat", "chat3", 512, -1, NULL, 0, 2, 1 },
	{ "MenuMove", "stnmv4", 32, -1, NULL, 0, 2, 1 }, //?????????
	{ "ClockTick", "tictoc", 32, -1, NULL, 0, 2, 1 },
	{ "Fireball", "impact3", 32, -1, NULL, 0, 2, 1 },
	{ "PuppyBeat", "pup4", 30, -1, NULL, 0, 2, 1 },
	{ "MysticIncant", "puzslv", 32, -1, NULL, 0, 4, 1 }
	//rww end
};
