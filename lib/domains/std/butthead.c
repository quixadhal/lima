/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit LIVING;
inherit M_ACTIONS;
inherit M_TRIGGERS;

void make_a_joke()
{
  do_game_command("say Beavis knows how to play an instrument.");
  do_game_command("say he plays the skin flute.");
  do_game_command("huhuh");
}

void initiate_beavis()
{ 
  object beavis = present("beavis", environment(this_object()));
  if(!beavis)
    {
      beavis = new(__DIR__ "beavis");
      beavis->move(environment(this_object()));
      simple_action("Beavis strolls out of the shadows, laughing about "
		  "his nads.\n"); 
    }
  beavis->follow_the_script_dumbass();

}



create() {
  
    ::create();
    set_name("Butthead");
    set_gender(1);
    set_proper_name("Butthead");
    set_in_room_desc("Butthead is here, scoping for chicks."); 
    set_long("His body odor prevents you from getting too close.");
    add_script("example", ({
1, (: initiate_beavis :),
1, (: make_a_joke :),
4, "kick beavis in the nads",
}));

    add_pattern("%slaugh%s",(:run_script("example"):));
}

