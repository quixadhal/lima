/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mudlist.c -- mudlist handling for the Intermud-3 daemon
**
** 95-May-19.  Deathblade.  Created.
** 95-July-2.  Rust.        Added a few interface functions.
*/

private int	mudlist_id;
private mapping	mud_info = ([ ]);

static private mapping	mud_names = ([ ]);
static private function	remap_name =
	(: lower_case(replace_string($1, " ", ".")) :);

static nomask int query_mudlist_id()
{
    return mudlist_id;
}

nomask string canon_mudname(string mudname)
{
    return mud_names[evaluate(remap_name, mudname)];
}

static nomask void rcv_mudlist(string orig_mud, string orig_user,
			       string targ_user, mixed * message)
{
    string mudname;
    mixed * info;

    mudlist_id = message[0];

    foreach ( mudname, info in message[1] )
    {
	if ( !info )
	    map_delete(mud_info, mudname);
	else
	    mud_info[mudname] = info;
    }

    mud_names = ([ ]);
    map_array(keys(mud_info),
	      (: $(mud_names)[evaluate(remap_name, $1)] = $1 :));
}

nomask mapping query_mudlist()
{
    return copy(mud_info);
}

nomask string * query_mudnames()
{
    return keys(mud_info);
}

nomask string * query_up_muds()
{
  return filter(keys(mud_info), (: mud_info[$1][0] == -1 :));
}

nomask int mud_exists(string mudname)
{
    return canon_mudname(mudname) != 0;
}
