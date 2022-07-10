Title:			HexenDS
Version:		0.28
Author:			Rich Whitehouse http://www.telefragged.com/thefatal/
Updates by:		Dopefish http://vespenegas.com/hexen.html and Sektor http://gtamp.com/DS
Date:			2011-02-15
Additional thanks:	WinterMute and Dovoto
			Chishm (libfat)
			Damian Yerrick (gbfs)
			Ravensoft for releasing the Hexen code to the public (I wish
			they'd release the JK2/SoF2 source!).
			Everyone else who has constributed to the wealth of knowledge
			available for DS homebrewing
-------------------------------------------------------------------------------------
What you need:
 -The wad from the full version of Hexen (shareware not supported natively by code
  released by Raven, may be integrated in later versions).
 -A DS and a compatible flash cart

How to use:
- If your flash cart doesn't support automatic DLDI patching then download dlditool and the DLDI driver for your device from:
  http://dldi.drunkencoders.com
- Patch Hexen for your device and then copy it to anywhere on your card.
- Get hexen.wad version 1.1 MD5 abb033caf81e26f12a2103e1fa25453f
- Copy hexen.wad x:\data\hexen\hexen.wad or /data/hexen/hexen.wad
- Saves are written to /data/hexen/
- Launch Hexen and hopefully it can find hexen.wad
- On slot-1 devices, it can take a minute to load on some slow devices, there is no progress bar

2011-04-04 v0.29 - Sektor
-------------------------
Selecting quit should now return to hbmenu compatible loaders
Not tested but should be Sudokuhax/iEvo DSi mode compatible
There are still many Z_Malloc memory errors (need to reduce memory usage)

2010-05-23 v0.28 - elhobbs and Sektor
-------------------------------------
Changes to the source code so it compiles with the current libnds. elhobbs sound code was the main change.

2009-12-17 v0.27 - Dopefish
---------------------------
This is a modified verison of Sektor's dualHexen, a port of Hexen to the Nintendo DS. Note that Sektor's version is itself a modification of Rich Whitehouse's hexenDS. Mainly I was frustrated by not being able to strafe, but there aren't enough buttons to go around, so I had to move something to the touchscreen. While I was at it I made a couple other changes, and here we are.

My changes include:

* Saving/Loading games (however savegames are not compatible with the PC version
* Inventory manipulation via the touchscreen:
* Touch the image of the active inventory item to use it.
* Touch the empty area just to the right of the item to bring up the inventory bar.
* Touch an item in the inventory bar to make it the active item.
* If you have more than 7 items in inventory, select the one at the edge to scroll, then return to your inventory.
* This interface could stand to be improved, but it is functional.
* Altered control configuration:
A = fire
B = use (i.e. open doors)
Y = jump
X = cycle weapons
L = strafe left
R = strafe right
D-Pad = move forward/backwards, turn left/right
Touchscreen = mouselook, inventory

Touch screen sensitivity can be adjusted in the options menu.  There is a special
ds menu on the main menu which has a gamma option and other ds-specific things
(such as warp and give all cheats).

Source code:
Source is included. There are a lot of warnings I've been too lazy to fix, but
otherwise it's relatively clean. Most changes will be marked with rww or Sektor.

Add -D_HEXENDS_GBFS to the arm9 makefile to build the GBFS version

2007-01-30 v0.26 notes - Sektor
-------------------------------
-Updated to DLDI version of libfat for greater device compatiblity
-A and B now work on the menu (previously you had to push Start and Select)
-Added libcartreset - Quit option returns to menu on supported slot-2 devices
-Level warp now works on DS-X (unexpected side effect of newer devkitARM)
-DS LED blinking is stopped at startup because DS-X annoyingly leaves it on
-DS sleeps/powersaves when closed
-Saving and loading are still broken
-I need a better icon

2006-10-27 - v0.25 - Sektor
---------------------------
-Moved from devkitARM r17 to r19b
-Added libfat support. Hexen can finally be played on GBAMP!
-Changed controls so you don't have to use the touch screen
-Saving and loading are still broken

2006-03-04 - v0.2 - Rich Whitehouse
-----------------------------------
-Moved from devkitARM r14 to r17, fixing a lot of common bugs in the process (this
 should take care of the swapped screens problem some people have as well).
-General speedups. Lots of them.
-Low detail mode from Doom incorporated (made low detail versions of all of Hexen's
 rendering functions). ARM versions of col/span low detail rendering also included
 (which are around 13% faster than their GCC O3 versions).
-On-demand texture patch resizing, in order to lessen memory use (should solve any
 remaining Z_Malloc errors, especially with those huge animating textures).
-Fixed the info screen displaying unreadable junk on the main screen (but it's
 still info from the PC version, just think of it as authentic!).
-Added an invert look option in the menus.
-Tweaked touchscreen code more to make it more errorproof (it has always worked fine
 on my hardware, so this isn't 100%).
-When errors occur, the game will display them instead of stopping and spinning in
 place, so that you can go to my forums and yell at me about them (if someone else
 hasn't already).
-Fixed mana bars not rendering empty space on the bottom screen.
-Proper loading screen for the initial load sequence.
-Removed the need for cat from the wad-building process.

2005-08-10 - v0.1 - Rich Whitehouse
--------------------------------
-First release. Includes sound, touchscreen and automap support.