/* Do not remove the headers from this file! see /USAGE for more info. */

//### this entire file is a FIXME. -Beek

/*
** parsedsc.c
**
** Parser for room description files (*.dsc).
** Inspired by discussions between Beek and Deathblade on 14-Nov-94.
**
** 14-Nov-94. Created. Deathblade.
*/

#include <driver/type.h>
#include <mudlib.h>

/*
** Constant definition stuff
*/
private static mapping basic_obs;
private static mapping funcs;

/*
** Parsing context
*/
private static object * cur_obs;
private static int cur_indent;


private void parse_dsc_line(string line)
{
    object parent;
    object ob;
    string path;
    int colon;

    line = trim_spaces(line);
    if ( line[0] == '#' || line[0] == 0 )
	return;

    /*
    ** ### for now, ignore the 0-indent line. code below also assumes
    ** ### that line[0] == '.' and therefore cur_indent > 0.
    */
    if ( line[0] != '.' )
	return;

    /*
    ** Make cur_indent point to the first non-. character.
    */
    if ( line[cur_indent] == '.' )
    {
	while ( line[++cur_indent] == '.' )
	    ;
    }
    else
    {
	while ( line[--cur_indent] != '.' )
	    ;
	++cur_indent;
    }

    parent = cur_obs[cur_indent - 1];
    switch ( line[cur_indent] )
    {
    case '<':
	path = basic_obs[line[cur_indent..<2]];
	if ( !path )
	    path = OBJ;
	ob = new(path);
	ob->move(parent);
	break;

    case '[':
	write("### '[' form not supported yet ###\n");
	break;

    case '(':
	colon = member_array(':', line);
	if ( colon == -1 )
	{
	    write("### missing colon in items defn ###\n");
	}
	else
	{
	    parent->add_items(([ explode(line[cur_indent+1 .. colon-1], ",") :
			       line[colon+1 ..] ]));
	}
	break;

    default:
	colon = member_array(':', line);
	if ( colon == -1 )
	{
	    path = trim_spaces(line[cur_indent ..]);
	    if ( path[0] != '/' )
	    {
		/* ### bad way to do this, but I'm lazy... */
		path = implode(explode(file_name(), "/")[0 .. <2], "/") +
		    "/" + path;
	    }

	    /*
	    ** Find the ob and move it into the parent if it doesn't
	    ** already have a location.
	    */
	    ob = load_object(path);
	    if ( !environment(ob) )
		ob->move(parent);
	}
	else
	{
	    write("### ':' form not supported yet ###\n");
	}
	break;
    }
}

nomask void parse_flags(object ob, string arg)
{
    /* ### */
}

private nomask void init_data()
{
    /* ### may not be able to use these if we need M_GETTABLE, too */
    basic_obs = ([
		 "OBJ" : OBJ,
		 "CONTAINER" : CONTAINER,
		 ]);

    funcs = ([
	     "proper_name" : ({ "set_proper_name", STRING }),
	     "long" : ({ "set_long", STRING }),
	     "adj" : ({ "set_adjectives", ARRAY }),
	     "weight" : ({ "set_weight", INT }),
	     "gettable" : ({ "set_gettable", INT }),
	     "flags" : ({ (: parse_flags :), STRING }),
	     "in_room" : ({ "set_in_room_desc", STRING }),
	     "untouched" : ({ "set_untouched_desc", STRING }),
	     ]);
}

varargs nomask void parse_dsc(string fname)
{
    string dsc;

    dsc = read_file(fname ? fname : file_name() + ".dsc");
    if ( !dsc )
	return;

    cur_obs = allocate(10);
    cur_obs[0] = this_object();

    cur_indent = 0;

    init_data();

    map_array(explode(dsc, "\n"), (: parse_dsc_line :));
}
