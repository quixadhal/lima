/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/domains/std/spells/stock-mage/std_mage_spell";

void setup ()
{
    set_spell_name("frotz");
    set_cast_time(3);
}

int get_spell_level()
{
    return 1;
}

mixed valid_target(object ob)
{
    if ( !ob )
    {
	return "You need to specify what you want to cast frotz on.\n";
    }

    return 1;
}

void do_effects(object target, object reagent)
{
    int old_light = target->query_light();

    if ( old_light > 0 )
    {
	this_body()->simple_action("Although the $o $v1are already lit, "
				   "$n $vtry to light $o.\n", target);
	return 0;
    }

    target->adjust_light(-old_light + 1);

    if (target->query_light() > 0)
    {
	this_body()->simple_action("$N $vstare at the $o intensely for a brief moment.  Suddenly, $o is surrounded by a sickly light, just bright enough to see by!\n", target);
    }
    else 
    {
	this_body()->simple_action("$N $vstare at the $o intensely for a brief moment.\n", target);
	write("Nothing happens.\n");
    }
}
