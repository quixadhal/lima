/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** guilds.c -- player's information about their guilds
**
** Ohara: Design and original implementation
** 951224, Deathblade: updated, installed, etc
*/

class guild_info
{
    int member_level;
    int suspend_level;
    int banned_level;
}


private mapping guilds = ([ ]);


private nomask void check_guild_security()
{
    if ( this_object() == this_body() || check_previous_privilege(1))
	return;
    error( "Illegal attempt to set guild privilege" );
}


int add_guild( string name, int level )
{
    class guild_info gi;

    check_guild_security();
    GUILD_D->guild_check( name );

    if( (gi = guilds[name]) )
    {
	if ( gi->member_level )
	    return -1;

	gi->member_level = level;
	return 1;
    }
    else
    {
	gi = guilds[name] = new( class guild_info );
    }

    gi->member_level = level;

    return 1;
}

int revoke_guild( string name )
{
    class guild_info gi = guilds[ name ];

    check_guild_security();
    GUILD_D->guild_check( name );

    if( !gi )
    {
	return -1;
    }
    if( !(gi->member_level))
    {
	return 0;
    }
    gi->member_level = 0;
    if ( !gi->suspend_level && !gi->banned_level )
	map_delete(guilds, name);
    return 1;
}


int suspend_guild( string name, int level )
{
    class guild_info gi = guilds[ name ];

    check_guild_security();
    GUILD_D->guild_check( name );

    if( !gi )
    {
	return -1;
    }
    if( gi->suspend_level )
    {
	return 0;
    }
    gi->suspend_level = level;
    return 1;
}


int unsuspend_guild( string name )
{
    class guild_info gi = guilds[ name ];

    check_guild_security();
    GUILD_D->guild_check( name );

    if( !gi )
    {
	return -1;
    }
    if( !gi->suspend_level )
    {
	return 0;
    }
    gi->suspend_level = 0;
    return 1;
}


int ban_guild( string name, int level )
{
    class guild_info gi = guilds[ name ];

    check_guild_security();
    GUILD_D->guild_check( name );

    if( !gi )
    {
	return -1;
    }
    if( gi->banned_level )
    {
	return 0;
    }
    gi->banned_level = level;
    return 1;
}


int query_member_guild( string name )
{
    class guild_info gi = guilds[ name ];

    if( !gi )
    {
	return 0;
    }
    return gi->member_level;
}


int query_suspend_guild( string name )
{
    class guild_info gi = guilds[ name ];

    if( !gi )
    {
	return 0;
    }
    return gi->suspend_level;
}


int query_banned_guild( string name )
{
    class guild_info gi = guilds[ name ];

    if( !gi )
    {
	return 0;
    }
    return gi->banned_level;
}


string array guilds_belong()
{
    return keys(filter_mapping(guilds,
	(: ((class guild_info)$2)->member_level :)));
}

void clear_guilds()
{
    guilds = ([ ]);
    return;
}
