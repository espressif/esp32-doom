/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
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
 *      Startup and quit functions. Handles signals, inits the
 *      memory management, then calls D_DoomMain. Also contains
 *      I_Init which does other system-related startup stuff.
 *
 *-----------------------------------------------------------------------------
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"
#include "m_fixed.h"
#include "i_system.h"
#include "i_video.h"
#include "z_zone.h"
#include "lprintf.h"
#include "m_random.h"
#include "doomstat.h"
#include "g_game.h"
#include "m_misc.h"
#include "i_sound.h"
#include "i_main.h"
#include "r_fps.h"
#include "lprintf.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/* Most of the following has been rewritten by Lee Killough
 *
 * I_GetTime
 * killough 4/13/98: Make clock rate adjustable by scale factor
 * cphipps - much made static
 */

int realtic_clock_rate = 100;
static int_64_t I_GetTime_Scale = 1<<24;

static int I_GetTime_Scaled(void)
{
  return (int)( (int_64_t) I_GetTime_RealTime() * I_GetTime_Scale >> 24);
}



static int  I_GetTime_FastDemo(void)
{
  static int fasttic;
  return fasttic++;
}



static int I_GetTime_Error(void)
{
  I_Error("I_GetTime_Error: GetTime() used before initialization");
  return 0;
}



int (*I_GetTime)(void) = I_GetTime_Error;

void I_Init(void)
{
  /* killough 4/14/98: Adjustable speedup based on realtic_clock_rate */
  if (fastdemo)
    I_GetTime = I_GetTime_FastDemo;
  else
    if (realtic_clock_rate != 100)
      {
        I_GetTime_Scale = ((int_64_t) realtic_clock_rate << 24) / 100;
        I_GetTime = I_GetTime_Scaled;
      }
    else
      I_GetTime = I_GetTime_RealTime;

  {
    /* killough 2/21/98: avoid sound initialization if no sound & no music */
    if (!(nomusicparm && nosfxparm))
      I_InitSound();
  }

  R_InitInterpolation();
}


/* killough 2/22/98: Add support for ENDBOOM, which is PC-specific
 *
 * this converts BIOS color codes to ANSI codes.
 * Its not pretty, but it does the job - rain
 * CPhipps - made static
 */

inline static int convert(int color, int *bold)
{
  if (color > 7) {
    color -= 8;
    *bold = 1;
  }
  switch (color) {
  case 0:
    return 0;
  case 1:
    return 4;
  case 2:
    return 2;
  case 3:
    return 6;
  case 4:
    return 1;
  case 5:
    return 5;
  case 6:
    return 3;
  case 7:
    return 7;
  }
  return 0;
}

/* CPhipps - flags controlling ENDOOM behaviour */
enum {
  endoom_colours = 1,
  endoom_nonasciichars = 2,
  endoom_droplastline = 4
};

unsigned int endoom_mode;

static void PrintVer(void)
{
  char vbuf[200];
  lprintf(LO_INFO,"%s\n",I_GetVersionString(vbuf,200));
}


static int has_exited;

/* I_SafeExit
 * This function is called instead of exit() by functions that might be called
 * during the exit process (i.e. after exit() has already been called)
 * Prevent infinitely recursive exits -- killough
 */

void I_SafeExit(int rc)
{
  if (!has_exited)    /* If it hasn't exited yet, exit now -- killough */
    {
      has_exited=rc ? 2 : 1;
      exit(rc);
    }
}

#ifdef SECURE_UID
uid_t stored_euid = -1;
#endif

//int main(int argc, const char * const * argv)
int doom_main(int argc, char const * const *argv)
{
#ifdef SECURE_UID
  /* First thing, revoke setuid status (if any) */
  stored_euid = geteuid();
  if (getuid() != stored_euid)
    if (seteuid(getuid()) < 0)
      fprintf(stderr, "Failed to revoke setuid\n");
    else
      fprintf(stderr, "Revoked uid %d\n",stored_euid);
#endif

  myargc = argc;
  myargv = argv;

#ifdef _WIN32
  if (!M_CheckParm("-nodraw")) {
    /* initialize the console window */
    Init_ConsoleWin();
    atexit(Done_ConsoleWin);
  }
#endif
  /* Version info */
  lprintf(LO_INFO,"\n");
  PrintVer();

  /* cph - Z_Close must be done after I_Quit, so we register it first. */
  atexit(Z_Close);
  /*
     killough 1/98:

     This fixes some problems with exit handling
     during abnormal situations.

     The old code called I_Quit() to end program,
     while now I_Quit() is installed as an exit
     handler and exit() is called to exit, either
     normally or abnormally. Seg faults are caught
     and the error handler is used, to prevent
     being left in graphics mode or having very
     loud SFX noise because the sound card is
     left in an unstable state.
  */

  Z_Init();                  /* 1/18/98 killough: start up memory stuff first */

  I_SetAffinityMask();

  /* cphipps - call to video specific startup code */
  I_PreInitGraphics();

  D_DoomMain ();
  return 0;
}
