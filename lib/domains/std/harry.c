/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit LIVING;
inherit M_ACTIONS;
inherit M_TRIGGERS;

string *query_channel_list() {
    return ({ "plyr_gossip" });
}

create() {
    int *handle;
    
    ::create();
    set_name("Harry");
    set_gender(1);
    set_proper_name("Harry the affectionate");
    set_in_room_desc("Harry the affectionate");
    add_id("dude");
    set_long("Harry has an agreeable look.");
    set_actions( 10, ({
      "say What are you waiting for?",
      "say Hello there!",
      "say I don't like winter.",
      "say I don't like snow.",
      "say I don't like rain.",
      "say Who are you?",
      "say Why do you look like that?",
      "say What are you doing here?",
      "say Nice weather, isn't it?",
      "smile",
      "lsfbog",
      "ack",
//      "chan gossip Is this thing on?",
      "rock"
    }) );
    add_pattern("%s smiles.", (: $1 != "Harry the affectionate" ? "smile" : 0 :));
    add_pattern("%s has left the game.", (: "say Why did " + $1
					  + " quit the game ?" :));
    add_pattern("%s gets %s.", (: "say Why did " + $1
				+ " take " + $2 + " ?" :));
    add_pattern("%s drops %s.", (: "say Why did " + $1
				 + " drop " + $2 + " ?" :));
    add_pattern("%s kicks you.", (: "kick " + $1 :));
    add_pattern("%s kisses you.", "blush");
    add_pattern("%s nods.", function() {
	respond("nod sol*");
	respond("think did I miss something?");
    } );
    add_pattern("%s laughs.", "say Did I miss a joke?");
    add_pattern("%s chuckles.", "smirk");
    add_pattern("%s boggles.", "bog");
    add_pattern("%s enters", (: "say Hi " + $1 + ", nice to see you !" :) );
    add_pattern("%s leaves %s.\n", (: "go " + $2 :));

    // This subrule says that the right hand side will give us a string
    // we should sprintf() the name into, with the additional stipulation
    // that we ignore other Harry's
    add_pattern("%s says: %s.", (: ($2 && $1 != "Harry the affectionate")
					   ? sprintf($2, $1)
					   : 0 :),
                 0, "say_rule");
    add_sub_pattern("say_rule", "yes", "say I disagree");
    add_sub_pattern("say_rule", "right", "no", "say I disagree");
    add_sub_pattern("say_rule", "shut up",
                    "say Why do you want me to shut up?");
    add_sub_pattern("say_rule", 
		    "%shello%s", "say Pleased to meet you!");
    add_sub_pattern("say_rule", 
		    "%sLPC%s", "turing %s");
    add_sub_pattern("say_rule", 
                    "%sstay here%s", "say Ok then.");
    add_sub_pattern("say_rule", 
                    "%snot follow%s", "say Ok then.");
    add_sub_pattern("say_rule", 
                    "%sget lost%s", "say Ok then.");
    add_sub_pattern("say_rule", 
                    "%s", (: random(5) == 0 
                       ? "say Why do you say '" + $1 + "'???" 
                       : 0 :));
}
