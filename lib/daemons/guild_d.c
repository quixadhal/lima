/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** guild_d.c -- daemon for recording guild information2
**
** Ohara: Design and original implementation
** 951224, Deathblade: updated, installed, etc
*/

inherit M_DAEMON_DATA;

class guild_defn
{
    string        guild_prospectus;	/* info about the guild */
    int           guild_begone;		/* what to do upon leaving guild */
    int           guild_suspend_level;	/* suspend level upon suspension */
    string array  guild_attributes;	/* the guild's attributes */
    string array  guild_exclusive;	/* exclusive attributes */
    string array  guild_allies;		/* allied guilds */
    string array  guild_prereq;		/* prerequisite guilds */
    string array  guild_banned;		/* exclusive guilds */
    string        guild_title;		/* formal title of this guild */
    int           guild_sees_secret;	/* can see other guilds' attrs */
    int           guild_is_secret;	/* attrs are (generally) secret */
    int           guild_need_all;	/* need ALL prereq guilds */
}


private mapping guilds = ([ ]);

#define DEFN(name)	((class guild_defn)guilds[name])

#define SAVE_FILE	"/data/daemons/guild_d"
#define PRIV_NEEDED	"Mudlib"	// owner priv implies Arch-type


void set_guild_title( string name, string title );
void set_guild_banned( string name, string array banned );
void set_guild_prereq( string name, string array prereq );
void set_guild_allies( string name, string array allies );
void set_guild_exclusive( string name, string array exclusive );
void set_guild_attributes( string name, string array attributes );
void set_guild_begone( string name, int begone );
void set_guild_suspend_level( string name, int suspend_level );
void set_guild_prospectus( string name, string prospectus );
void set_guild_sees_secret( string name, int sees_secret );
void set_guild_is_secret( string name, int is_secret );
void set_guild_need_all( string name, int need_all );


string query_guild_title( string name );
string array query_guild_banned( string name );
string array query_guild_prereq( string name );
string array query_guild_allies( string name );
string array query_guild_exclusive( string name );
string array query_guild_attributes( string name );
int query_guild_begone( string name );
int query_guild_suspend_level( string name );
string query_guild_prospectus( string name );
int query_guild_sees_secret(string name);
int query_guild_is_secret(string name);
int query_guild_need_all(string name);


void guild_add( string name )
{
    class guild_defn gi;

    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    gi = new( class guild_defn );
    gi->guild_title = "Title Error";
    gi->guild_prospectus = "We Have Nothing To Offer";
    gi->guild_suspend_level = 1;
    guilds[ name ] = gi;
    save_me();
}


void guild_remove( string name )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    map_delete(guilds, name);
    save_me();
}


void remove_all_guilds()
{
if(!check_previous_privilege( PRIV_NEEDED))
    error( "Insufficient priv to remove all guilds.");
    guilds = ([]);
    save_me();
    write( "Guilds Wiped.\n");
}

void guild_check(string name)
{
    if ( guilds[name] )
	return;

    error("Non-existant guild - " + name + ".\n");
}


void set_guild_title( string name, string title )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_title = title;
    save_me();
}


void set_guild_banned( string name, string array banned... )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_banned = banned;
    save_me();
}


void set_guild_prereq( string name, string array prereq... )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_prereq = prereq;
    save_me();
}


void set_guild_allies( string name, string array allies... )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_allies = allies;
    save_me();
}


void set_guild_exclusive( string name, string array exclusive... )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_exclusive = exclusive;
    save_me();
}


void set_guild_attributes( string name, string array attributes... )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_attributes = attributes;
    save_me();
}


void set_guild_begone( string name, int begone )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_begone = begone;
    save_me();
}


void set_guild_suspend_level( string name, int suspend_level )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_suspend_level = suspend_level;
    save_me();
}


void set_guild_prospectus( string name, string prospectus )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_prospectus = prospectus;
    save_me();
}


void set_guild_sees_secret( string name, int sees_secret )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_sees_secret = sees_secret;
    save_me();
}


void set_guild_is_secret( string name, int is_secret )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_is_secret = is_secret;
    save_me();
}


void set_guild_need_all( string name, int need_all )
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("insufficient privilege");

    guild_check(name);
    DEFN(name)->guild_need_all = need_all;
    save_me();
}


string query_guild_title( string name )
{
    guild_check(name);
    return DEFN(name)->guild_title;
}


string array query_guild_banned( string name )
{
    guild_check(name);
    return DEFN(name)->guild_banned;
}


string array query_guild_prereq( string name )
{
    guild_check(name);
    return DEFN(name)->guild_prereq;
}


string array query_guild_allies( string name )
{
    guild_check(name);
    return DEFN(name)->guild_exclusive;
}


string array query_guild_exclusive( string name )
{
    guild_check(name);
    return DEFN(name)->guild_exclusive;
}


string array query_guild_attributes( string name )
{
    guild_check(name);
    return DEFN(name)->guild_attributes;
}


int query_guild_begone( string name )
{
    guild_check(name);
    return DEFN(name)->guild_begone;
}


int query_guild_suspend_level( string name )
{
    guild_check(name);
    return DEFN(name)->guild_suspend_level;
}


string query_guild_prospectus( string name )
{
    guild_check(name);
    return DEFN(name)->guild_prospectus;
}


int query_guild_sees_secret( string name )
{
    guild_check(name);
    return DEFN(name)->guild_sees_secret;
}


int query_guild_is_secret( string name )
{
    guild_check(name);
    return DEFN(name)->guild_is_secret;
}


int query_guild_need_all( string name )
{
    guild_check(name);
    return DEFN(name)->guild_need_all;
}


/* given the player's current guilds, can they join the new one? */
mixed guild_is_ok( string name, string array player_guilds )
{
    string array prereq;
    string array banned;
    string array exclusive;
    int sees_secret;

    guild_check(name);

    /* check the new guild's prerequisites against the current guilds */
    prereq = DEFN(name)->guild_prereq;
    if ( prereq )
    {
	string array common = prereq & player_guilds;

	/* do we have to have ALL the prereqs? or just one? */
	if ( DEFN(name)->guild_need_all )
	{
	    if ( sizeof(common) != sizeof(prereq) )
	    {
//### should return the missing prereqs
//### missing = prereq - common
		return "not all prerequisites have been filled";
	    }
	}
	else if ( !sizeof(common) )
	{
//### should return the list of prereqs
	    return "need to fill one of the prerequisites";
	}
    }
    else
    {
	/* initialize as empty just in case the "banned" check needs it */
	prereq = ({ });
    }

    /*
    ** If the banned list contains the single guild "all" then all guilds
    ** are banned (the player cannot be in ANY guilds except the guilds
    ** that are prerequisites).
    */
    banned = DEFN(name)->guild_banned;
    if ( sizeof(banned) && banned[0] == "all" &&
	 sizeof(player_guilds - prereq) )
    {
	return "cannot join while a member of other guilds";
    }

    exclusive = DEFN(name)->guild_exclusive;
    sees_secret = DEFN(name)->guild_sees_secret;

    foreach ( string item in player_guilds )
    {
	string * attributes;

	/* if this item is in the new guild's banned list, punt */
	if ( banned && member_array(item, banned) != -1 )
	{
	    return DEFN(item)->guild_title;
	}

	/* if the guild in question has secret attrs and we can't see them,
	   then skip it and go on to the next guild */
	if ( !sees_secret && DEFN(item)->guild_is_secret )
	    continue;

	/* if any attributes are in common between the excluded list and
	   the guild's attributes, then punt */
	attributes = DEFN(item)->guild_attributes;
	if ( exclusive && attributes && sizeof(exclusive & attributes) > 0 )
	{
	    return DEFN(item)->guild_title;
	}
    }

    return 0;
}


void define_from_file(string fname)
{
    string * lines;
    string which_guild;

    lines = filter(explode(read_file(fname), "\n"),
		   (: $1[0] != '#' :));

    foreach ( string line in lines )
    {
	int idx = member_array('=', line);
	string attr;
	mixed value;

	/* hmm. malformed. just skip it -- probably blank. */
	if ( idx == -1 )
	    continue;

	attr = trim_spaces(line[0..idx-1]);
	value = trim_spaces(line[idx+1..]);

	if ( attr == "name" )
	{
	    write("Defining '" + value + "'...\n");
	    guild_add(value);
	    which_guild = value;
	    continue;
	}

	if ( !which_guild )
	    error("attribute specified before guild name\n");

	switch ( attr )
	{
	case "prospectus":
	    set_guild_prospectus(which_guild, value);
	    break;
	case "begone":
	    set_guild_begone(which_guild, to_int(value));
	    break;
	case "suspend_level":
	    set_guild_suspend_level(which_guild, to_int(value));
	    break;
	case "attributes":
	    value = map(explode(value, ","), (: trim_spaces :));
	    set_guild_attributes(which_guild, value...);
	    break;
	case "exclusive":
	    value = map(explode(value, ","), (: trim_spaces :));
	    set_guild_exclusive(which_guild, value...);
	    break;
	case "allies":
	    value = map(explode(value, ","), (: trim_spaces :));
	    set_guild_allies(which_guild, value...);
	    break;
	case "prereq":
	    value = map(explode(value, ","), (: trim_spaces :));
	    set_guild_prereq(which_guild, value...);
	    break;
	case "banned":
	    value = map(explode(value, ","), (: trim_spaces :));
	    set_guild_banned(which_guild, value...);
	    break;
	case "title":
	    set_guild_title(which_guild, value);
	    break;
	case "sees_secret":
	    set_guild_sees_secret(which_guild, to_int(value));
	    break;
	case "is_secret":
	    set_guild_is_secret(which_guild, to_int(value));
	    break;
	case "need_all":
	    set_guild_need_all(which_guild, to_int(value));
	    break;
	}
    }
}

string * query_guilds()
{
    return keys(guilds);
}
