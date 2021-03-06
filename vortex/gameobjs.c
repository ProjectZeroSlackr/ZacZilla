/*
 * gameobjs
 *
 *   Scott Lawrence
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  $Id: gameobjs.c,v 1.1 2005/05/16 02:53:00 yorgle Exp $
 *
 *  $Log: gameobjs.c,v $
 *  Revision 1.1  2005/05/16 02:53:00  yorgle
 *  Initial checkin of Vortex.  It's just a demo right now, soon to be fleshed out.
 *  Remove the "Vortex Demo" line(92) from menu.c to disable the hook.
 *
 */

#include "../pz.h"
#include "../vectorfont.h"


/* various objects, needed for vortex game objects */
VECTOR_POLY_STRUCT V_gameobjs[] =
{   
    /* CLAW -- Player ship */
	{ { 6, 2, 7, 17, 20, 19, 21, 18, 6 }, { 1, 1, 1, 1,  1, 1, 1, 1 } },
};

