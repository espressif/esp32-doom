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
 *  Misc system stuff needed by Doom, implemented for Linux.
 *  Mainly timer handling, and ENDOOM/ENDBOOM.
 *
 *-----------------------------------------------------------------------------
 */

#include <stdio.h>

#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#ifdef _MSC_VER
#define    F_OK    0    /* Check for file existence */
#define    W_OK    2    /* Check for write permission */
#define    R_OK    4    /* Check for read permission */
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>



#include "config.h"
#include <unistd.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "m_argv.h"
#include "lprintf.h"
#include "doomtype.h"
#include "doomdef.h"
#include "lprintf.h"
#include "m_fixed.h"
#include "r_fps.h"
#include "i_system.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"

int realtime=0;

int I_StartDisplay(void)
{
  return true;
}

void I_EndDisplay(void)
{
}

void I_uSleep(unsigned long usecs)
{
}


int I_GetTime_RealTime (void)
{
  return realtime++;
}

fixed_t I_GetTimeFrac (void)
{
  return 0;
}

void I_GetTime_SaveMS(void)
{
}

unsigned long I_GetRandomTimeSeed(void)
{
	return 4;
}

const char* I_GetVersionString(char* buf, size_t sz)
{
  sprintf(buf,"%s v%s (http://prboom.sourceforge.net/)",PACKAGE,VERSION);
  return buf;
}

const char* I_SigString(char* buf, size_t sz, int signum)
{
  return buf;
}

unsigned char *doom1waddata;

typedef struct {
	unsigned char *mem;
	int offset;
	int size;
} FileDesc;


static FileDesc fds[64];


int I_Open(const char *wad, int flags) {
	int x=3;
	while (fds[x].mem!=NULL) x++;
	if (strcmp(wad, "DOOM1.WAD")==0) {
		fds[x].mem=doom1waddata;
		fds[x].offset=0;
		fds[x].size=3370127; //ToDo: de-hardcode - JD
	} else {
		lprintf(LO_INFO, "I_Open: open %s failed\n", wad);
		return -1;
	}
	return x;
}

int I_Lseek(int ifd, off_t offset, int whence) {
	if (whence==SEEK_SET) {
		fds[ifd].offset=offset;
	} else if (whence==SEEK_CUR) {
		fds[ifd].offset+=offset;
	} else if (whence==SEEK_END) {
		lprintf(LO_INFO, "I_Lseek: SEEK_END unimplemented\n");
	}
	return fds[ifd].offset;
}


void I_Read(int ifd, void* vbuf, size_t sz)
{
	if (fds[ifd].offset+sz>fds[ifd].size) {
		sz=fds[ifd].size-fds[ifd].offset;
	}
	memcpy(vbuf, fds[ifd].mem+fds[ifd].offset, sz);
	fds[ifd].offset+=sz;
}

int I_Filelength(int ifd)
{
	return fds[ifd].size;
}


void *I_Mmap(void *addr, size_t length, int prot, int flags, int ifd, off_t offset) {
	lprintf(LO_INFO, "I_Mmap: mmapped offset %d\n", (int)offset);
	return fds[ifd].mem+offset;
}


int I_Munmap(void *addr, size_t length) {
	return 0;
}


const char *I_DoomExeDir(void)
{
  return "";
}



char* I_FindFile(const char* wfname, const char* ext)
{
  char *p;
  p = malloc(strlen(wfname)+4);
  sprintf(p, "%s.%s", wfname, ext);
  return NULL;
}

void I_SetAffinityMask(void)
{
}


int access(const char *path, int atype) {
    return 1;
}
