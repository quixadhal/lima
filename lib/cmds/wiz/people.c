/* Do not remove the headers from this file! see /USAGE for more info. */

//  Rust 2-6-94
#include <playerflags.h>
#include <levels.h>
#include <mudlib.h>

inherit CMD;


#define DIVIDER \
"-------------------------------------------------------------------------\n"

private void main()
{
    object* user_obs;
    object* bodies;
    int i;
    string name;
    mixed info;
    int bits;
    string where;
    object o;
    string extra;

    user_obs = users();

    bodies = map_array(user_obs, (: $1->query_body() :)) - ({0});
    bodies = sort_array(bodies, (: $1->query_level() == $2->query_level() ?
		       $1->query_score() > $2->query_score() :
		       $1->query_level() > $2->query_level() :) );
    write(DIVIDER);
    printf("%|70s\n", implode(explode(mud_name(),"")," "));
    printf("%|70s\n", "(PST is: "+ctime(time())+")");
    if ( extra )
	printf("%|70s\n",extra);
    write(DIVIDER);
    i = sizeof(bodies);
    if(!i)
	printf("%-17s%4s     %15s  %s\n","[(*SATAN*)]","none","666.666.666.666","HELL!!!!!");
    while ( i-- )
    {
      info = bodies[i]->query_who_data();
	bits = info[0];
	name = info[1];
	if(!name)name = "login";
	else
	    name = capitalize(name);
	if( bodies[i] && bodies[i]->test_flag(F_INVIS) ) name = "("+name+")";
	if(bits & (1 << FlagIndex(F_HIDDEN))) name = "["+name+"]";
	if( bodies[i] && (o = environment(bodies[i])) )
	    where = file_name(o);
	else
	    where = "(null)";
	if(!o) name = "<"+name+">";
	printf("%-10s %-13s%4d %c%c %-s\n",
	       LEVELS[info[3]],
	       name,
	       bodies[i] ? bodies[i]->query_score() : 0,
	       (query_idle(bodies[i]->query_link()) > 60 ? 'I':' '),
	       (bodies[i] && bodies[i]->test_flag(F_IN_EDIT) ? 'E' : ' '),
	       where);
    }
    write(DIVIDER);
    return;
}


int help()       
{            
    write(wrap(  
	"Usage: people [-p|z | -w|i | -l | -m]\n"
	"Shows relevent info of people on the mud.  The default is all on line "
	"users, but the p or z flag will restrict this to players only, and the w and i "
	"flags will restrict people to show wizards only. "
	"The l flag will show all living creatures, and the m flag will show "
	"all monsters, but no interactive users.  people -l and people -m "
	"can give inacurate information if there has been a recent update "
	"of the simul_efun object.\n"
      ));          
    return 1;    
}
