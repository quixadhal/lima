/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** banish_d.c Jan 31, 1994 by Rust
**
** whacked so many times since then... not really near the original,
** but hey...
**
** 961209, Deathblade: updated to use classes and to record reasons in the
**                     data file (rather than just the log)
*/

#include <log.h>

inherit M_DAEMON_DATA;
inherit M_GLOB;

inherit CLASS_BANISH_DATA;

class banish_data * bad_names = ({ });
class banish_data * bad_sites = ({ });

//### used to upgrade old banish information
private void create()
{
    ::create();

    if ( sizeof(bad_names) != 0 && stringp(bad_names[0]) )
    {
	for ( int i = sizeof(bad_names); i--; )
	    bad_names[i] = new(class banish_data,
			       item: bad_names[i],
			       reason: "unknown; check the log");
	for ( int i = sizeof(bad_sites); i--; )
	    bad_sites[i] = new(class banish_data,
			       item: bad_sites[i],
			       reason: "unknown; check the log");
	save_me();
    }
}
    
void banish_name(string name, string reason)
{
    if ( !check_privilege("Mudlib:daemons") )
	return;

    if ( !stringp(name) || !stringp(reason) )
	error("bad name or reason for banishing\n");

    name = lower_case(trim_spaces(name));
    if ( name == "" )
	error("bad name for banishing\n");

    LOG_D->log(LOG_BANISH,
	       sprintf("%s banished the name \"%s\" because: %s\n",
		       this_user()->query_userid(),
		       name,
		       reason));

    bad_names += ({ new(class banish_data, item: name, reason: reason) });
    save_me();
}

void unbanish_name(string name)
{
    if ( !check_privilege("Mudlib:daemons") )
	return;

    bad_names = filter(bad_names, (: ((class banish_data)$1)->item != $(name) :));
    save_me();
}

void banish_site(string site, string reason)
{
    if ( !check_privilege("Mudlib:daemons") )
	return;

    if ( !stringp(site) || !stringp(reason) )
	error("bad site or reason for banishing\n");

    site = lower_case(trim_spaces(site));
    if ( site == "" )
	error("bad site for banishing\n");

    LOG_D->log(LOG_BANISH,
	       sprintf("%s banished the site \"%s\" because: %s\n",
		       this_user()->query_userid(),
		       site, 
		       reason));

    bad_sites += ({ new(class banish_data, item: site, reason: reason) });
    save_me();
}

void unbanish_site(string site)
{
    if ( !check_privilege("Mudlib:daemons") )
	return;

    bad_sites = filter(bad_sites, (: ((class banish_data)$1)->item != $(site) :));
    save_me();
}

int check_name(string name)
{
    // return non-zero if banished
    return sizeof(filter(bad_names,
			 (: regexp($(name),
				   ((class banish_data)$1)->item) :)));
}

int check_site(string *check)
{
    /* allow check to be passed in for debugging purposes */
    if ( !check )
	check = ({ query_ip_number(previous_object()), 
		   lower_case(query_ip_name(previous_object())) });

    // return non-zero if banished
    return sizeof(filter(bad_sites,
			 (: sizeof(regexp($(check),
					  translate(((class banish_data)$1)->item))) :)));
}

mixed *show_banishes()
{
    return copy(({ bad_names, bad_sites }));
}
