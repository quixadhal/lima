/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SPELL;

void setup()
{
    set_spell_name("unlock");
}

void cast_spell(object ob, object reagent)
{
    mixed result = ob->magic_unlock();

    if ( stringp(result) )
    {
	this_body()->simple_action("$N $vcast a spell on $o...", ob);
	write(result);
    }
    else if ( !result )
    {
	this_body()->simple_action("$N $vtry to unlock $o with a spell, but fails.", ob);
    }
    else
    {
	this_body()->simple_action("$N $vunlock $o with a spell.", ob);
    }
}

mixed valid_target(object ob) 
{
    /* the target must be lockable() */
    return ob && ob->is_lockable();
}

mixed valid_reagent(object ob)
{
    return !ob || "That wouldn't help the spell any.\n";
}

mixed valid_circumstances(mixed target, mixed reagent)
{
    if ( !target )
	return "You need to cast the spell on something.\n";
    if ( reagent )
	return "That won't help your unlock spell.\n";
    return 1;
}
