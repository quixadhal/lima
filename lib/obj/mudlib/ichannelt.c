/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit "/std/living/grammar";

int subj_obj;
string mudname, user_name;

void setup( mixed who, int s_o )
{
    if ( !who ) return ;

    subj_obj = s_o;
    
    if ( stringp( who ) )
    {
	string used_mudname;
	sscanf( who, "%s@%s", user_name, used_mudname );

	if ( used_mudname )
	  mudname = IMUD_D->canon_mudname( used_mudname );

	if ( mudname )
	{
	    int gend = IMUD_D->get_gender( mudname, user_name );

	    // No idea if this is silent or not ...
#if 0
	    if ( this_body() ) move( this_body(), "in" );
#else
	    // This is probably safer -Beek
	    move_object(this_body());
#endif

	    set_id( "wibblewibble"+lower_case(user_name) + " "+
		   replace_string( lower_case(used_mudname), ".", " " ) );
	    set_gender( gend == -1 ? 1 : gend ); // Assume male if unknown

	} else
	  remove();
    } else
    {
	set_gender( who->query_gender() );
	user_name = who->query_userid();
    }

    // we exist temporarily only
    call_out( (: remove :), 0);
}

int is_living() { return 1; }
string query_mud_name() { return mudname; }
string query_userid() { return user_name; }
string query_name() { return capitalize( user_name ); }

string short()
{
    return ( subj_obj & 1 ? "$N" :
	    ( subj_obj & 2 ? "$O" :
	     query_name()+( mudname ? "@"+mudname : "" ) ) );
}
string a_short() { return short(); }
string the_short() { return short(); }

