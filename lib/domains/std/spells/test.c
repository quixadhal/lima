/* Do not remove the headers from this file! see /USAGE for more info. */


inherit SPELL;

void setup()
{
    set_spell_name("test");
}

void cast_spell(object ob, object reagent)
{
    if ( !ob )
    {
	this_body()->simple_action("$N $vcast $p test spell.");
    }
    else
    {
	this_body()->targetted_action("$N $vcast $p test spell on $t.", ob);
    }
}

mixed valid_target(object ob) 
{
    return 1;
}

mixed valid_reagent(object ob)
{
    return "That wouldn't help the spell any.\n";
}
  
// returns 1 by default, which isn't what you want.  Which is why you
// should subclass spell...
int valid_circumstances(object target, object reagent)
{
    return wizardp(this_body());
}
