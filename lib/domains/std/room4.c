/* Do not remove the headers from this file! see /USAGE for more info. */

 /*
*** Example weather room  by Yaynu @ Lima Bean 160895
*/
 
inherit OUTDOOR_ROOM;
 
void setup()
{
set_area("wiz_area");
    set_brief("Environmental room");
    set_long("The weather seems to change here a lot. Harry hates this \
place.");
    set_weather(1);
    
    set_exits( ([
    "west": "room2",
    ]) );
}   


