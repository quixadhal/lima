/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_lie_wrd_obj(string prep, object ob)
{
   environment(this_body())->do_go_obj(ob, prep);
}

void do_lie_down()
{
   if(this_body()->lie_down())
      this_body()->simple_action("$N $vlie down on the ground.");
}

void do_lie()
{
   this_body()->simple_action( "$N $vclaim to have a brilliant wit, and a sparkling personality.");
}

void create()
{
   add_rules( ({ "WRD OBJ", "", "down" }) );
}
