/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SPELL;
inherit CLASS_COMBAT_RESULT;

void setup()
{
    set_spell_name("fireball");
}

void cast_spell(object ob, object reagent)
{

  object array targets;
    if ( !ob )
    {
    targets = filter(all_inventory(environment(this_body())),
		(: $1 != this_body() :));
	this_body()->simple_action("$N $vcast a fireball spell!\n");
    }
    else
    {
    targets = ({ ob });
	this_body()->targetted_action("$N $vcast a fireball spell at $t1.\n", ob);
    }

  foreach( object item in targets)
    {
	/* okay... we won't have the fireball hit the caster... :-) */
	if ( item == this_body() )
	    continue;

	//reduce hit points here
	item->do_damage(new(class combat_result,
			    kind : "fire",
			    damage : 5,
			    message : "$T $v1scream in pain!\n"));
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
