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
 *  Low level UDP network interface. This is shared between the server
 *  and client, with SERVER defined for the former to select some extra
 *  functions. Handles socket creation, and packet send and receive.
 *
 *-----------------------------------------------------------------------------*/

# include "config.h"
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#include <stdlib.h>
#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#ifdef HAVE_NET


#include "protocol.h"
#include "i_network.h"
#include "lprintf.h"

void I_ShutdownNetwork(void)
{
}

void I_InitNetwork(void)
{
}

UDP_PACKET *I_AllocPacket(int size)
{
}

void I_FreePacket(UDP_PACKET *packet)
{
}

void I_WaitForPacket(int ms)
{
}

int I_ConnectToServer(const char *serv)
{
  return 0;
}

void I_Disconnect(void)
{
}

UDP_SOCKET I_Socket(Uint16 port)
{
}

void I_CloseSocket(UDP_SOCKET sock)
{
}

UDP_CHANNEL I_RegisterPlayer(IPaddress *ipaddr)
{
}

void I_UnRegisterPlayer(UDP_CHANNEL channel)
{
}

/*
 * ChecksumPacket
 *
 * Returns the checksum of a given network packet
 */
static byte ChecksumPacket(const packet_header_t* buffer, size_t len)
{
  const byte* p = (const void*)buffer;
  byte sum = 0;

  if (len==0)
    return 0;

  while (p++, --len)
    sum += *p;

  return sum;
}

size_t I_GetPacket(packet_header_t* buffer, size_t buflen)
{
  return 0;
}

void I_SendPacket(packet_header_t* packet, size_t len)
{
}

void I_SendPacketTo(packet_header_t* packet, size_t len, UDP_CHANNEL *to)
{
}

void I_PrintAddress(FILE* fp, UDP_CHANNEL *addr)
{
}

#endif /* HAVE_NET */
