/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * A monster object which can use the 'go' verb properly
 * 042998 Created by Tigran
 */

//:MODULE
//
// MOVING_MONSTER enables the use of the 'go' verb in npcs (via 
// do_game_command()) by inheriting M_SMARTMOVE.  
inherit MONSTER;
inherit M_SMARTMOVE;

/* Maybe something more should be here, but I couldn't think of anything
 * yet.  Mostly this was set up to allow triggers M_WANDER and M_FOLLOW
 * to use it w/ proper inhertance. -- Tigran.*/
