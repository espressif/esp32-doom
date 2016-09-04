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

int joyleft;
int joyright;
int joyup;
int joydown;

int usejoystick;


static void I_EndJoystick(void)
{
  lprintf(LO_DEBUG, "I_EndJoystick : closing joystick\n");
}

void I_PollJoystick(void)
{
#if 0
  event_t ev;
  Sint16 axis_value;

  if (!usejoystick || (!joystick)) return;
  ev.type = ev_joystick;
  ev.data1 =
    (SDL_JoystickGetButton(joystick, 0)<<0) |
    (SDL_JoystickGetButton(joystick, 1)<<1) |
    (SDL_JoystickGetButton(joystick, 2)<<2) |
    (SDL_JoystickGetButton(joystick, 3)<<3);
  axis_value = SDL_JoystickGetAxis(joystick, 0) / 3000;
  if (abs(axis_value)<10) axis_value=0;
  ev.data2 = axis_value;
  axis_value = SDL_JoystickGetAxis(joystick, 1) / 3000;
  if (abs(axis_value)<10) axis_value=0;
  ev.data3 = axis_value;

  D_PostEvent(&ev);
#endif
}

void I_InitJoystick(void)
{
}
