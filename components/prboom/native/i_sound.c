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
 *  System interface for sound.
 *
 *-----------------------------------------------------------------------------
 */

#include "config.h"
#include <math.h>
#include <unistd.h>


#include "z_zone.h"

#include "m_swap.h"
#include "i_sound.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "lprintf.h"
#include "s_sound.h"

#include "doomdef.h"
#include "doomstat.h"
#include "doomtype.h"

#include "d_main.h"

int snd_card = 0;
int mus_card = 0;
int snd_samplerate = 0;


void I_UpdateSoundParams(int handle, int volume, int seperation, int pitch)
{
}


void I_SetChannels(void)
{
}

int I_GetSfxLumpNum(sfxinfo_t* sfx)
{
    return 1;
}

int I_StartSound(int id, int channel, int vol, int sep, int pitch, int priority)
{
  return channel;
}



void I_StopSound (int handle)
{
}


int I_SoundIsPlaying(int handle)
{
    return 0;
}


int I_AnySoundStillPlaying(void)
{
  return false;
}



void I_ShutdownSound(void)
{
}

//static SDL_AudioSpec audio;

void I_InitSound(void)
{
}




void I_ShutdownMusic(void)
{
}

void I_InitMusic(void)
{
}

void I_PlaySong(int handle, int looping)
{
}

extern int mus_pause_opt; // From m_misc.c

void I_PauseSong (int handle)
{
}

void I_ResumeSong (int handle)
{
}

void I_StopSong(int handle)
{
}

void I_UnRegisterSong(int handle)
{
}

int I_RegisterSong(const void *data, size_t len)
{
  return (0);
}

int I_RegisterMusic( const char* filename, musicinfo_t *song )
{
    return 1;
}

void I_SetMusicVolume(int volume)
{
}


