/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust 2-6-94

#include <mudlib.h>
#include <playerflags.h>

inherit CMD;

#define DIVIDER \
"-------------------------------------------------------------------------\n"

#ifdef ZORKMUD
# define USER_DESC	"(ZORKERS ONLY)"
# define WHO_FORMAT	"%s:  (GUE Time is: %s) %28s\n%s"
#else
# define USER_DESC	"(PLAYERS ONLY)";
# define WHO_FORMAT	"%s:  (Local Time is: %s) %28s\n%s"
#endif


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
		extra = USER_DESC;
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

    retval += sprintf(WHO_FORMAT, mud_name(), ctime(time()), extra, DIVIDER);
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

    out(get_who_string(arg));
}

void player_menu_entry()
{
  bare_init();
  main(0);
  done_outputing();
}
