/* Do not remove the headers from this file! see /USAGE for more info. */
//:TODO
//Fix give_new_obj to take create args; then fix inf supply
//to pass them.

inherit CONTAINER;

private string array obs = ({ });

void set_objects(mapping m) {
    foreach( string key, mixed value in m )
    {
	if( arrayp( value ))
	{
	    if( value[0] == -1 )
	    {
		m[key] = value[1..];
		obs += ({ absolute_path(key) });
	    }
	}
	else if( value == -1 )
	{
	    m[key] = 1;
	    obs += ({ absolute_path(key) });
	}
	::set_objects( m );
    }
}

mixed release_object(object target, int force) {
    mixed ret = ::release_object(target, force);
    string obj = absolute_path( base_name(target));

    if( ret == 1 && member_array( obj, obs ) != -1 )
	give_new_obj( this_object(), obj, 0 );

    return ret;
}

