/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust 2-6-94

#include <mudlib.h>
#include <playerflags.h>

#define DIVIDER \
"-------------------------------------------------------------------------\n"


int level_sort(object o1, object o2)
{
    return (o1->query_level() > o2->query_level());
}

string get_who_string(string arg)
{
    object *u;
    int    idle_int, bits, i;
    string idle, name, extra, retval;
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
            u = filter_array(u, (: !call_other($1, "test_flag", F_INVIS) :));
    }
    else
        u = filter_array(users(), (: !call_other($1, "test_flag", F_INVIS) :));
    u = sort_array(u, (: level_sort :));
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
        info = u[i]->query_who_data();
        bits = info[0];
	if (!u[i]->query_body())
	    continue;
        name = u[i]->query_body()->query_title();
        if(!name)
            name = capitalize(u[i]->query_real_name());
//        else
//            name = capitalize(name);
        if(bits & (1 << FlagIndex(F_INVIS)))
            name = "("+name+")";
        if(bits & (1 << FlagIndex(F_HIDDEN)))
            name = "["+name+"]";
        if(u[i]->test_flag(F_IN_EDIT))
            name = "*"+ name;
        idle_int = query_idle(u[i]);
        if(idle_int >= 3600)
            idle = sprintf("%dh",idle_int/3600);
        else if(idle_int >= 60) idle = sprintf("%dm",idle_int/60);
        else idle = "";
        retval += sprintf("%-68s %=4s\n",name,idle);
    }
    return retval + DIVIDER;
}

int main(string arg)
{
  if( arg == "" )
    arg = 0;
        write(get_who_string(arg));
    return 1;
}
