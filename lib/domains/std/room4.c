/* Do not remove the headers from this file! see /USAGE for more info. */

 /*
*** Example weather room  by Yaynu @ Lima Bean 160895
*/
 
#include <mudlib.h>
inherit ROOM;
 
create(){
               
    room::create();
    set_brief("Environmental room");
    set_light(1);
    set_long("The weather seems to change here a lot. Harry hates this \
place.");
    set_weather(1);
    
    exits = ([
    "west": "/domains/std/room2",
            ]);
}   
