/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** drive.c
**
** Used to drive vehicles
**
** 951123, Deathblade: created
*/

inherit NVERB_OB;

//###should be shared somehow with go.c
string array normal_directions = ({ "north", "south", "east", "west",
                 "northwest", "northeast", "southwest", "southeast" });

mixed can_drive_str(string str)
{
   mixed value = environment(environment(this_body()))->can_go_str(str);
   if(!stringp(value) && (value == 1))
     return default_checks();
   if(!stringp(value) && (member_array(str, normal_directions) != -1))
      return "It doesn't appear you can drive that way.\n";
   return value;
}

void do_drive_str(string str)
{
  environment(environment(this_body()))->do_go_str(str);
}

mixed can_drive_obj()
{
   return "You need to specify a direction.\n";
}
 
mixed can_drive_obj_str(object ob, string str)
{
   mixed value = environment(this_body())->can_go_somewhere(str);
   if(!stringp(value) && (value == 1))
      return default_checks();
   if(!stringp(value) && (member_array(str, normal_directions) != -1))
      return "It doesn't appear you can go that way.\n";
   return value;
}

void do_drive_obj_str(object ob, string str)
{
   ob->do_go_somewhere(str);
}

void create()
{
    add_rules( ({ "STR", "OBJ", "OBJ STR" }) );
}
