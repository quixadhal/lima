/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

// Rust/Belboz
private void main( mixed *arg)
{

    object      start_place, end_place;
    object	target;
    string	target_name;

    target = arg[0];
    start_place = environment( target );
    target_name = target->query_name();
    end_place = environment( this_body());
    if( start_place == end_place )
    {
	if( this_body()->query_name() == target_name )
	{
	    out( "You can't trans yourself.\n");
	    return;
	}
	out( capitalize( target_name ) + " is already here.\n" );
	return;
    }
    target->move( end_place );
    if( environment( target ) == start_place )
    {
	out( "Trans:  move failed.\n" );
	return;
    }
    if( ! target_name )
	target_name = target->short();

    /* Be careful here or it will be impossible to trans someone with no env() */
    if (!(target->query_link() && interactive(target->query_link()) && target_name == "Someone") && start_place)
	tell_from_inside( start_place , sprintf( "%s dissapears in a puff of smoke.\n",
	    target_name ) );
    outf( "You summon %s to you.\n", target_name );
    tell( target, "You are magically transported somewhere.\n" );

    if( !( target->query_link() && interactive(target->query_link()) &&
	target_name == "Someone" ) )
    {
	tell_environment( target, sprintf("%s arrives in a puff of smoke.\n",
	    target_name ), 0, ({ target, this_body() }) );
    }

    target->force_look();
    return;
}

