/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_wade()
{
  mixed s = environment(this_body())->wade();

  if(stringp(s))
    {
      write(s);
      return;
    }
  else if(!s)
    {
      write("I don't understand what you want to wade in.\n");
      return;
    }
}

void do_wade_wrd_obj(string prep,object ob)
{
  ob->do_verb_rule("wade", "WRD OBJ", prep, ob);
}

void create()
{
    add_rules ( ({ "", "WRD OBJ", }),({  }) );

}
