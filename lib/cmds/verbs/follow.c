/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

mixed direct_follow_obj(object ob)
{
  if(!present(ob,environment(this_body() ) ) )
    {
      this_body()->my_action("$N $vis not here.",ob);
      return 0;
    }
  return 1;
}

void do_follow_obj(object ob) 
{
  this_body()->do_follow_obj(ob);
}

mixed map_func(mixed element)
{
  if(objectp(element))
    return element->short();
  if(stringp(element))
    return element;
  return "";
}

void do_follow()
{
  string array follows=map_array(this_body()->query_follow_search(),(: map_func :) );
  follows-=({""});
  if(!sizeof(follows))
    {
      write("You are not following anything.\n");
      return;
    }
  if(sizeof(follows)==1)
    {
      write("You are following "+follows[0]+".\n");
      return;
    }
  write("You are following: "+implode(follows[0..<2],", ")+" and "+follows[<1]+ ".\n");
}
				 
void create() 
{
  add_rules( ({""}) );
  add_rules( ({ "OBJ" }) );
}
