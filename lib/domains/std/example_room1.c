/* Do not remove the headers from this file! see /USAGE for more info. */
/*                                                             */
/*   This is an example room for the lima mudlib          */
/*                                                             */
/*   Created by zifnab@lima bean 7/24/95                         */
/*                                                             */
/****************************************************************/

#include <mudlib.h>
inherit ROOM;

void setup(){
  set_area("wiz_area");
    set_brief("Example Room");
 
    set_long("This is an example room for the lima mudlib.  It is fairly non-desscript however there are a few objects laying about that may warrant looking at. Propped up in the southwest corner is an old discarded suit of armor.  On the west side of the room is an arched doorway leading bask to the wizard lounge. For some odd reason there is a car wash to the east!\n");
 
    set_exits( ([
	"west" : "wizroom",
	"south" : "room2",
	"north" : "/domains/std/rooms/v_plains/4/7",
	"east" : (: environment(this_body())->is_vehicle() && member_array("car", environment(this_body())->query_id()) != -1 ? "/domains/std/car_wash.c" : "#Perhaps it would be better if you were in a car!\n" :),
    ]) );
    set_exit_msg("east", "As you enter the car wash, jets of water begin washing the under carriage of the car. \n");
 
    add_item( "suit" , "armor" , "suit of armor" , "The armor is very old and well worn.  While examining the armor you imagine you hear the sounds of combat coming from behind you.");

    set_objects ( ([
		    "/domains/std/safe.c" : 1 
		    ]));
}

