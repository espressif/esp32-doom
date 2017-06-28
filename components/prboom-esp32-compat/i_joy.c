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
 *   Joystick handling for Linux
 *
 *-----------------------------------------------------------------------------
 */

#ifndef lint
#endif /* lint */

#include <stdlib.h>

#include "doomdef.h"
#include "doomtype.h"
#include "m_argv.h"
#include "d_event.h"
#include "d_main.h"
#include "i_joy.h"
#include "lprintf.h"

#include "psxcontroller.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


volatile int joyVal=0;
int oldJoyVal;

int usejoystick;
int joyleft;
int joyright;
int joyup;
int joydown;

void I_EndJoystick(void)
{
  lprintf(LO_DEBUG, "I_EndJoystick : closing joystick\n");
}

void I_PollJoystick(void)
{
	int newJoyVal=joyVal;
	event_t ev;
	ev.type = ev_joystick;
	ev.data1=0;
	ev.data2=0;
	ev.data3=0;
	if ((newJoyVal&0x4000)==0) ev.data1|=1;
	if ((newJoyVal&0x2000)==0) ev.data1|=2;
	if ((newJoyVal&0x8000)==0) ev.data1|=4;
	if ((newJoyVal&0x1000)==0) ev.data1|=8;

	if ((newJoyVal&0x20)==0) ev.data2=120;
	if ((newJoyVal&0x80)==0) ev.data2=-120;

	if ((newJoyVal&0x40)==0) ev.data3=120;
	if ((newJoyVal&0x10)==0) ev.data3=-120;


	oldJoyVal=newJoyVal;
	D_PostEvent(&ev);

}


void jsTask(void *arg) {
	printf("Joystick task starting.\n");
	while(1) {
		vTaskDelay(20/portTICK_PERIOD_MS);
		joyVal=psxReadInput();
//		if (joyVal!=oldJoyVal) printf("Joy: %x\n", joyVal^0xffff);
		oldJoyVal=joyVal;
	}
}

extern int     joybfire;
extern int     joybstrafe;
extern int     joybuse;
extern int     joybspeed;

void I_InitJoystick(void)
{
	lprintf(LO_INFO, "I_InitJoystick: Initializing game pad.\n");
	usejoystick=1;
	joyleft=1;
	joyright=2;
	joyup=3;
	joydown=4;

	joybfire=1;
	joybuse=2;
	joybstrafe=3;
	joybspeed=0;
}

void jsInit() {
	//Starts the js task
	psxcontrollerInit();
	xTaskCreatePinnedToCore(&jsTask, "js", 5000, NULL, 7, NULL, 0);
}

