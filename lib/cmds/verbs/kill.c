/* Do not remove the headers from this file! see /USAGE for more info. */


#include <mudlib.h>

inherit VERB_OB;

void do_kill_liv(object ob)
{
    if (ob == this_body())     
	this_body()->commit_suicide();
    else
	this_body()->start_fight(ob);
}

void do_kill() {
    string err;
    
    if (err = this_body()->continue_fight()) {
	write(err);
    }
}

mixed * query_verb_info()
{
   return ({ ({ "", "LIV", "LIV with OBJ" }) });
}

