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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "z_zone.h"

#include "m_swap.h"
#include "i_sound.h"
#include "m_misc.h"
#include "w_wad.h"
#include "lprintf.h"
#include "s_sound.h"

#include "doomdef.h"
#include "doomstat.h"
#include "doomtype.h"

#include "d_main.h"

#include "m_fixed.h"

#include "sndhw.h"

#include "dbopl.h"

extern int sound_inited;

int snd_card=1;
int mus_card=1;
#define RATE (22050)
int snd_samplerate=RATE;

typedef int32_t fixed_pt_t; //24.8 fixed point format
#define TO_FIXED(x) ((x)<<8)
#define FROM_FIXED(x) ((x)>>8)

typedef struct {
	uint8_t *samp; //NULL if slot is disabled
	int vol;
	fixed_pt_t rate_inc; //every output sample, pos is increased by this
	fixed_pt_t len;
	fixed_pt_t pos;
} snd_slot_t;

#define NO_SLOT 4

snd_slot_t slot[NO_SLOT];

typedef struct {
	uint8_t reg;
	uint8_t data;
	uint16_t delay;
} imf_packet_t;

//imf is 700Hz tick
#define IMF_RATE 700

typedef struct {
	imf_packet_t *imf;
	int pos;
	int len;
	int delay_to_go;
	Chip opl;
} imf_player_t;

static imf_player_t imfplayer;


static void imf_player_tick(int samps) {
	if (!imfplayer.imf) return;
	while (imfplayer.delay_to_go < samps) {
		//handle next imf packet
		imfplayer.pos++;
		if (imfplayer.pos==imfplayer.len) imfplayer.pos=0;
		Chip__WriteReg(&imfplayer.opl, imfplayer.imf[imfplayer.pos].reg, imfplayer.imf[imfplayer.pos].data);
		samps-=imfplayer.delay_to_go;
		imfplayer.delay_to_go=(imfplayer.imf[imfplayer.pos].delay*RATE)/IMF_RATE;
	}
	imfplayer.delay_to_go-=samps;
}


static void snd_cb(int16_t *buf, int len) {
	static int32_t *oplblk=NULL;
	if (!oplblk) {
		oplblk=calloc(len, sizeof(int32_t));
		assert(oplblk);
	}
	imf_player_tick(len);
	Chip__GenerateBlock2(&imfplayer.opl, len, oplblk);
	for (int p=0; p<len; p++) {
		int samp=oplblk[p]*4; //mix in music
		for (int i=0; i<NO_SLOT; i++) {
			if (slot[i].samp) {
				//mix in sound fx slot
				samp+=(slot[i].samp[FROM_FIXED(slot[i].pos)])*128;
				//increase, unload if end
				slot[i].pos+=slot[i].rate_inc;
				if (slot[i].pos > slot[i].len) {
//					printf("Slot %d done\n", i);
					slot[i].samp=NULL;
				}
			}
		}
#if 0
		samp=samp/NO_SLOT;
#else
		if (samp<-32768) samp=32768;
		if (samp>32767) samp=32767;
#endif
		buf[p]=samp;
	}
}

typedef struct {
	uint16_t format_no;
	uint16_t samp_rate;
	uint32_t samp_ct;
	uint8_t pad[16];
	uint8_t samples[0];
} dmx_samp_t;

static void namebuf_upper(char *namebuf) {
	for (int i=0; i<8; i++) {
		if (namebuf[i]>='a' && namebuf[i]<='z') {
			namebuf[i]-=32;
		}
	}
}

int lumpnum_for_sndid(int id) {
	char namebuf[9];
	sprintf(namebuf, "ds%s", S_sfx[id].name);
	namebuf_upper(namebuf);
	int r=W_GetNumForName(namebuf);
//	printf("lumpnum_for_sndid: id %d is %s -> lump %d\n", id, namebuf, r);
	return r;
}


int I_StartSound(int id, int channel, int vol, int sep, int pitch, int prio) {
	if ((channel < 0) || (channel >= NO_SLOT))
		return -1;

	dmx_samp_t *snd=(dmx_samp_t*)W_CacheLumpNum(lumpnum_for_sndid(id));
	if (snd->format_no!=3) {
		printf("I_StartSound: unknown format %d\n", snd->format_no);
		return -1;
	}
	sndhw_lock();
	slot[channel].samp=NULL;
	slot[channel].vol=vol;
	slot[channel].rate_inc=TO_FIXED(snd->samp_rate)/RATE;
	slot[channel].len=TO_FIXED(snd->samp_ct);
	slot[channel].pos=0;
	slot[channel].samp=&snd->samples[0];
	sndhw_unlock();
	return channel;
}

void I_ShutdownSound(void)
{
    if (sound_inited) {
		lprintf(LO_INFO, "I_ShutdownSound: ");
		lprintf(LO_INFO, "\n");
        sound_inited = false;
	}
}

//static SDL_AudioSpec audio;

void I_InitSound(void) {
	I_InitMusic();
	sndhw_init(RATE, snd_cb);

	// Finished initialization.
    lprintf(LO_INFO,"I_InitSound: sound ready");
}


void I_InitMusic(void) {
	DBOPL_InitTables();
	Chip__Chip(&imfplayer.opl);
	Chip__Setup(&imfplayer.opl, RATE);
	imfplayer.imf=NULL;
}

void I_PlaySong(int handle, int looping) {
    if(handle == mus_None) return;
	
	sndhw_lock();
	char namebuf[9];
	sprintf(namebuf, "d_%s", S_music[handle].name);
	namebuf_upper(namebuf);
	printf("STUB: I_PlaySong %d (%s)\n", handle, namebuf);
	int lump=W_GetNumForName(namebuf);
	imfplayer.imf=(imf_packet_t*)W_CacheLumpNum(lump);
	imfplayer.pos=0;
	imfplayer.len=W_LumpLength(lump)/sizeof(imf_packet_t);
	imfplayer.delay_to_go=0;
	sndhw_unlock();
}


void I_PauseSong (int handle){
//	printf("STUB: I_PauseSong %d\n", handle);
}

void I_ResumeSong (int handle) {
//	printf("STUB: I_ResumeSong %d\n", handle);
}

void I_StopSong(int handle) {
//	printf("STUB: I_StopSong %d\n", handle);
}

void I_SetMusicVolume(int volume) {
//	printf("STUB: I_SetMusicVolume %d\n", volume);
}
