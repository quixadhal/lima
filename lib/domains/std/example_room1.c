/* Do not remove the headers from this file! see /USAGE for more info. */
/*                                                             */
/*   This is an example room for the lima mudlib          */
/*                                                             */
/*   Created by zifnab@lima bean 7/24/95                         */
/*                                                             */
/****************************************************************/

#include <mudlib.h>
inherit ROOM;

create(){
 
    room::create();
 
    set_brief("Example Room");
 
set_long("This is an example room for the lima mudlib.  It is fairly non-desscript however there are a few objects laying about that may warrant looking at. Propped up in the southwest corner is an old discarded suit of armor.  On the west side of the room is an arched doorway leading bask to the wizard lounge.");
 
    set_light(1);
 
    exits = ([
    "west" : "/domains/std/wizroom",
    "south" : "/domains/std/room2"
  ]);
 
    set_items ( ([

	  ({ "suit" , "armor" , "suit of armor" }) : "The armor is very old and well worn.  While examining the armor you imagine you hear the sounds of combat coming from behind you" ]) );

    set_objects ( ([
		    "/domains/std/sign.c" : 1,
		    "/domains/std/safe.c" : 1 
		    ]));
}

