/* Do not remove the headers from this file! see /USAGE for more info. */


inherit SPELL;

void create ()
{
  set_spell_name ("test");
  create_spell ();
}

void cast_spell (object ob, object reagent)
{

  if (!ob)
    {
      this_body ()->simple_action ("$N $vcast $p test spell.\n");
    }
  else
    {
      this_body ()->targetted_action ("$N $vcast $p test spell on $t.\n", ob);
    }
}

mixed valid_target (object ob) 
{
  return 1;
}

mixed valid_reagent (object ob)
{
  return "That wouldn't help the spell any.\n";
}
  


int valid_circumstances (object target, object reagent) // returns 1 by default, which
// isn't what you want.  Which is why you should subclass spell...
{
  return wizardp (this_body());
}


