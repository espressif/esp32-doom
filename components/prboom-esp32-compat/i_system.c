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
#include "i_joy.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_partition.h"
#include "esp_spi_flash.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"

#include <sys/time.h>

int realtime=0;


void I_uSleep(unsigned long usecs)
{
	vTaskDelay(usecs/1000);
}

static unsigned long getMsTicks() {
  struct timeval tv;
  struct timezone tz;
  unsigned long thistimereply;

  gettimeofday(&tv, &tz);

  //convert to ms
  unsigned long now = tv.tv_usec/1000+tv.tv_sec*1000;
  return now;
}

int I_GetTime_RealTime (void)
{
  struct timeval tv;
  struct timezone tz;
  unsigned long thistimereply;

  gettimeofday(&tv, &tz);

  thistimereply = (tv.tv_sec * TICRATE + (tv.tv_usec * TICRATE) / 1000000);

  return thistimereply;

}

const int displaytime=0;

fixed_t I_GetTimeFrac (void)
{
  unsigned long now;
  fixed_t frac;


  now = getMsTicks();

  if (tic_vars.step == 0)
    return FRACUNIT;
  else
  {
    frac = (fixed_t)((now - tic_vars.start + displaytime) * FRACUNIT / tic_vars.step);
    if (frac < 0)
      frac = 0;
    if (frac > FRACUNIT)
      frac = FRACUNIT;
    return frac;
  }
}


void I_GetTime_SaveMS(void)
{
  if (!movement_smooth)
    return;

  tic_vars.start = getMsTicks();
  tic_vars.next = (unsigned int) ((tic_vars.start * tic_vars.msec + 1.0f) / tic_vars.msec);
  tic_vars.step = tic_vars.next - tic_vars.start;
}

unsigned long I_GetRandomTimeSeed(void)
{
	return 4; //per https://xkcd.com/221/
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

extern unsigned char *doom1waddata;

typedef struct {
	const esp_partition_t* part;
	int offset;
	int size;
} FileDesc;

static FileDesc fds[32];

int I_Open(const char *wad, int flags) {
	int x=3;
	while (fds[x].part!=NULL) x++;
	if (strcmp(wad, "DOOM1.WAD")==0) {
		fds[x].part=esp_partition_find_first(66, 6, NULL);
		fds[x].offset=0;
		fds[x].size=fds[x].part->size;
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

int I_Filelength(int ifd)
{
	return fds[ifd].size;
}

void I_Close(int fd) {
	fds[fd].part=NULL;
}


typedef struct {
	spi_flash_mmap_handle_t handle;
	void *addr;
	int offset;
	size_t len;
	int used;
} MmapHandle;

#define NO_MMAP_HANDLES 128
static MmapHandle mmapHandle[NO_MMAP_HANDLES];

static int nextHandle=0;
static int getFreeHandle() {
	int n=NO_MMAP_HANDLES;
	while (mmapHandle[nextHandle].used!=0 && n!=0) {
		nextHandle++;
		if (nextHandle==NO_MMAP_HANDLES) nextHandle=0;
		n--;
	}
	if (n==0) {
		lprintf(LO_ERROR, "I_Mmap: More mmaps than NO_MMAP_HANDLES!");
		exit(0);
	}
	
	if (mmapHandle[nextHandle].addr) {
		spi_flash_munmap(mmapHandle[nextHandle].handle);
		mmapHandle[nextHandle].addr=NULL;
//		printf("mmap: freeing handle %d\n", nextHandle);
	}
	int r=nextHandle;
	nextHandle++;
	if (nextHandle==NO_MMAP_HANDLES) nextHandle=0;

	return r;
}

static void freeUnusedMmaps() {
	for (int i=0; i<NO_MMAP_HANDLES; i++) {
		//Check if handle is not in use but is mapped.
		if (mmapHandle[i].used==0 && mmapHandle[i].addr!=NULL) {
			spi_flash_munmap(mmapHandle[i].handle);
			mmapHandle[i].addr=NULL;
			printf("Freeing handle %d\n", i);
		}
	}
}

void *I_Mmap(void *addr, size_t length, int prot, int flags, int ifd, off_t offset) {
	int i;
	esp_err_t err;
	void *retaddr=NULL;

	for (i=0; i<NO_MMAP_HANDLES; i++) {
		if (mmapHandle[i].offset==offset && mmapHandle[i].len==length) {
			mmapHandle[i].used++;
			return mmapHandle[i].addr;
		}
	}

	i=getFreeHandle();

	//lprintf(LO_INFO, "I_Mmap: mmaping offset %d size %d handle %d\n", (int)offset, (int)length, i);
	err=esp_partition_mmap(fds[ifd].part, offset, length, SPI_FLASH_MMAP_DATA, (const void**)&retaddr, &mmapHandle[i].handle);
	if (err==ESP_ERR_NO_MEM) {
		lprintf(LO_ERROR, "I_Mmap: No free address space. Cleaning up unused cached mmaps...\n");
		freeUnusedMmaps();
		err=esp_partition_mmap(fds[ifd].part, offset, length, SPI_FLASH_MMAP_DATA, (const void**)&retaddr, &mmapHandle[i].handle);
	}
	mmapHandle[i].addr=retaddr;
	mmapHandle[i].len=length;
	mmapHandle[i].used=1;
	mmapHandle[i].offset=offset;

	if (err!=ESP_OK) {
		lprintf(LO_ERROR, "I_Mmap: Can't mmap: %x (len=%d)!", err, length);
		return NULL;
	}

	return retaddr;
}


int I_Munmap(void *addr, size_t length) {
	int i;
	for (i=0; i<NO_MMAP_HANDLES; i++) {
		if (mmapHandle[i].addr==addr && mmapHandle[i].len==length) break;
	}
	if (i==NO_MMAP_HANDLES) {
		lprintf(LO_ERROR, "I_Mmap: Freeing non-mmapped address/len combo!");
		exit(0);
	}
//	lprintf(LO_INFO, "I_Mmap: freeing handle %d\n", i);
	mmapHandle[i].used--;
	return 0;
}

void I_Read(int ifd, void* vbuf, size_t sz)
{
	uint8_t *d=I_Mmap(NULL, sz, 0, 0, ifd, fds[ifd].offset);
	memcpy(vbuf, d, sz);
	I_Munmap(d, sz);
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




