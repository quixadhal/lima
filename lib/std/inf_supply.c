/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE
//inf_supply is a container which supports an unlimited supply of objects.
//If the number of objects to be created in set_objects() is -1 then one
//object is created and each time this object is removed/taken a new
//object is created.
//
//See set_objects

inherit CONTAINER;

private mapping obs = ([ ]);

mixed *set_objects(mapping m) {
    foreach( string key, mixed value in m )
    {
	if( arrayp( value ))
	{
	    if( value[0] == -1 )
	    {
		m[key][0] = 1;
		obs[absolute_path(key)] = m[key];
	    }
	}
	else if( value == -1 )
	{
	    m[key] = 1;
	    obs[absolute_path(key)] = -1;
	}
    }
    return ::set_objects( m );
}

mixed release_object(object target, int force) {
    mixed ret = ::release_object(target, force);
    string obj = absolute_path( base_name(target));

    if (ret == 1) {
        if (obs[obj] == -1)
            give_new_obj(this_object(), obj, 0);
	else if (arrayp(obs[obj]))
	    give_new_obj(this_object(), obj, 0, obs[obj]);
    }
    return ret;
}

