/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust 2-6-94

#include <mudlib.h>
#include <playerflags.h>

inherit CMD;

#define DIVIDER \
"-------------------------------------------------------------------------\n"


string get_who_string(string arg)
{
    object *u;
    int    bits, i;
    string name, extra, retval;
    mixed  info;

    extra = retval = "";
    if (this_user())
    {
        switch (arg)
        {
            case "-p":
            case "-z":
                u = filter_array(users(), (: !wizardp($1) :));
#ifdef ZORKMUD
                extra = "(ZORKERS ONLY)";
#else
                extra = "(PLAYERS ONLY)";
#endif
                break;
            case "-w":
            case "-i":
                u = filter_array(users(), (: wizardp :));
                extra = "(IMPLEMENTORS ONLY)";
                break;
            case "-l":
                if ( wizardp(this_user()) )
                {
                    u = users();
                    extra = "(ALL LIVINGS)";
                    break;
                }
            case "-m":
                if ( wizardp(this_user()) )
                {
                    u = filter_array(children(USER_OB),
				     (: !interactive($1) :));
                    extra = "(NON-INTERACTIVES)";
                    break;
                }
            default:
                if(arg)
                    retval += "Who: Unknown flag.\n";
                u = users();
        }
        if(!wizardp(this_user()))
	  u = filter_array(users(), (: $1->query_body()->is_visible() :));
    }
    else
	  u = filter_array(users(), (: $1->query_body()->is_visible() :));

#ifdef ZORKMUD
    retval += sprintf("%s:  (GUE Time is: %s) %28s\n%s",
#else
    retval += sprintf("%s:  (Local Time is: %s) %28s\n%s",
#endif
		      mud_name(), ctime(time()), extra, DIVIDER);
    i = sizeof(u);
    if(!i)
        retval += sprintf("%|70s\n","Sorry, no one fits that bill.");
    while(i--)
    {
	if (!u[i]->query_body())
	    continue;
        name = u[i]->query_body()->query_formatted_desc(78);
	/* ### temp until all people 'su' */
	if ( !name )
	    name = u[i]->query_body()->query_truncated_title(78);
        bits = u[i]->query_body()->get_flags(PLAYER_FLAGS);
        if(!name)
            name = capitalize(u[i]->query_userid());

        if(!(u[i]->query_body()->is_visible()))
            name = "("+name+")";
        if(bits & (1 << FlagIndex(F_HIDDEN)))
            name = "["+name+"]";
        if(u[i]->test_flag(F_IN_EDIT))
            name = "*"+ name;
        retval += sprintf("%-68s\n",name);
    }
    return retval + DIVIDER;
}

private void main(string arg)
{
  if( arg == "" )
    arg = 0;
        write(get_who_string(arg));
    return;
}
