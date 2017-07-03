/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2006 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *  DOOM graphics stuff for SDL
 *
 *-----------------------------------------------------------------------------
 */

#include "config.h"
#include <stdlib.h>
#include <unistd.h>
#include "m_argv.h"
#include "doomstat.h"
#include "doomdef.h"
#include "doomtype.h"
#include "v_video.h"
#include "r_draw.h"
#include "d_main.h"
#include "d_event.h"
#include "gamepad.h"
#include "i_video.h"
#include "z_zone.h"
#include "s_sound.h"
#include "sounds.h"
#include "w_wad.h"
#include "st_stuff.h"
#include "lprintf.h"

#include "rom/ets_sys.h"
#include "spi_lcd.h"

#include "esp_heap_alloc_caps.h"

int use_fullscreen=0;
int use_doublebuffer=0;


void I_StartTic (void)
{
	gamepadPoll();
}


static void I_InitInputs(void)
{
}



static void I_UploadNewPalette(int pal)
{
}

//////////////////////////////////////////////////////////////////////////////
// Graphics API

void I_ShutdownGraphics(void)
{
}

//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
}


void I_StartFrame (void)
{
}


int I_StartDisplay(void)
{
	spi_lcd_wait_finish();
  return true;
}

void I_EndDisplay(void)
{
}



static uint16_t *screena, *screenb;


//
// I_FinishUpdate
//

void I_FinishUpdate (void)
{
	uint16_t *scr=(uint16_t*)screens[0].data;
#if 0
	int x, y;
	char *chrs=" '.~+mM@";
	ets_printf("\033[1;1H");
	for (y=0; y<240; y+=4) {
		for (x=0; x<320; x+=2) {
			ets_printf("%c", chrs[(scr[x+y*320])>>13]);
		}
		ets_printf("\n");
	}
#endif
#if 1
	spi_lcd_send(scr);
#endif
	//Flip framebuffers
//	if (scr==screena) screens[0].data=screenb; else screens[0].data=screena;
}

int16_t lcdpal[256];

void I_SetPalette (int pal)
{
	int i, r, g, b, v;
	int pplump = W_GetNumForName("PLAYPAL");
	const byte * palette = W_CacheLumpNum(pplump);
	palette+=pal*(3*256);
	for (i=0; i<255 ; i++) {
		v=((palette[0]>>3)<<11)+((palette[1]>>2)<<5)+(palette[2]>>3);
		lcdpal[i]=(v>>8)+(v<<8);
//		lcdpal[i]=v;
		palette += 3;
	}
	W_UnlockLumpNum(pplump);
}


unsigned char *screenbuf;

#define INTERNAL_MEM_FB


void I_PreInitGraphics(void)
{
	lprintf(LO_INFO, "preinitgfx");
#ifdef INTERNAL_MEM_FB
	screenbuf=pvPortMallocCaps(320*240, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
	assert(screenbuf);
#endif
}


// CPhipps -
// I_SetRes
// Sets the screen resolution
void I_SetRes(void)
{
  int i;

//  I_CalculateRes(SCREENWIDTH, SCREENHEIGHT);

  // set first three to standard values
  for (i=0; i<3; i++) {
    screens[i].width = SCREENWIDTH;
    screens[i].height = SCREENHEIGHT;
    screens[i].byte_pitch = SCREENPITCH;
    screens[i].short_pitch = SCREENPITCH / V_GetModePixelDepth(VID_MODE16);
    screens[i].int_pitch = SCREENPITCH / V_GetModePixelDepth(VID_MODE32);
  }

  // statusbar
  screens[4].width = SCREENWIDTH;
  screens[4].height = (ST_SCALED_HEIGHT+1);
  screens[4].byte_pitch = SCREENPITCH;
  screens[4].short_pitch = SCREENPITCH / V_GetModePixelDepth(VID_MODE16);
  screens[4].int_pitch = SCREENPITCH / V_GetModePixelDepth(VID_MODE32);

//Attempt at double-buffering. Does not work.
//  free(screena);
//  free(screenb);
//  screena=malloc(SCREENPITCH*SCREENHEIGHT);
//  screenb=malloc(SCREENPITCH*SCREENHEIGHT);


#ifdef INTERNAL_MEM_FB
  screens[0].not_on_heap=true;
  screens[0].data=screenbuf;
  assert(screens[0].data);
#endif

//  spi_lcd_init();

  lprintf(LO_INFO,"I_SetRes: Using resolution %dx%d\n", SCREENWIDTH, SCREENHEIGHT);
}

void I_InitGraphics(void)
{
  char titlebuffer[2048];
  static int    firsttime=1;

  if (firsttime)
  {
    firsttime = 0;

    atexit(I_ShutdownGraphics);
    lprintf(LO_INFO, "I_InitGraphics: %dx%d\n", SCREENWIDTH, SCREENHEIGHT);

    /* Set the video mode */
    I_UpdateVideoMode();

    /* Initialize the input system */
    I_InitInputs();
	gamepadInit();

  }
}


void I_UpdateVideoMode(void)
{
  int init_flags;
  int i;
  video_mode_t mode;

  lprintf(LO_INFO, "I_UpdateVideoMode: %dx%d\n", SCREENWIDTH, SCREENHEIGHT);

//    mode = VID_MODE16;
    mode = VID_MODE8;

  V_InitMode(mode);
  V_DestroyUnusedTrueColorPalettes();
  V_FreeScreens();

  I_SetRes();

  V_AllocScreens();

  R_InitBuffer(SCREENWIDTH, SCREENHEIGHT);

}
