/* profile.c -- functions for profiling

   Copyright (C) 2002 Marko Mlinar, markom@opencores.org
   Copyright (C) 2008 Embecosm Limited

   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of Or1ksim, the OpenRISC 1000 Architectural Simulator.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* This program is commented throughout in a fashion suitable for processing
   with Doxygen. */


/* Autoconf and/or portability configuration */
#include "config.h"

/* System includes */
#include <stdio.h>

/* Package includes */
#include "profile.h"
#include "sim-config.h"
#include "arch.h"


/* Adds a new entry to the memory profile file */
void
mprofile (oraddr_t memaddr, unsigned char type)
{
  struct mprofentry_struct mp;
  mp.addr = memaddr;
  mp.type = type;
  if (!fwrite (&mp, sizeof (struct mprofentry_struct), 1, runtime.sim.fmprof))
    config.sim.mprofile = 0;
}
