/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SPELL;
inherit COMBAT_SPELL;

void setup()
{
   set_spell_name("fireball");
}

int calculate_damage()
{
   return random(5) + 1;
}

void cast_spell(object ob, object reagent)
{
    object array targets;

    if ( !ob )
    {
	targets = filter(all_inventory(environment(this_body())),
                         (: $1 != this_body() && $1->is_living() :));
	this_body()->simple_action("$N $vcast a fireball spell!");
    }
    else
    {
	targets = ({ ob });
	this_body()->targetted_action("$N $vcast a fireball spell at $t1.", ob);
    }

    foreach ( object item in targets )
    {
	/* okay... we won't have the fireball hit the caster... :-) */
	if ( item == this_body() )
	    continue;

	//reduce hit points here
        do_spell_damage(item, (: calculate_damage :));
    }
}

mixed valid_target(object ob) 
{
    /* a fireball can always be cast... */
    return 1;
}

mixed valid_reagent(object ob)
{
    return !ob || "That wouldn't help the spell any.\n";
}
