/* Do not remove the headers from this file! see /USAGE for more info. */

// banish_d.c Jan 31, 1992 by Rust  // Uh?  Don't think I was mudding then...
//  Try Jan 31, 1993.


#include <log.h>

inherit M_DAEMON_DATA;
inherit M_GLOB;

string* bad_names =({});
mixed* bad_sites = ({});

void banish_name(string name, string reason)
{
  if ( !check_previous_privilege(1) )
    return;

  if ( !stringp(name) )
      error("bad name for banishing\n");

  LOG_D->log(LOG_BANISH,
	     sprintf("%s banished the name %s (%s) because: %s\n",
		     this_user()->query_userid(), name,
		     ctime(time()), reason));

  bad_names += ({name});
  save_me();
}

void unbanish_name(string name)
{
  if ( !check_previous_privilege(1) )
    return;

  bad_names -= ({name});
  save_me();
}

void banish_site(string site, string reason)
{
  if ( !check_previous_privilege(1) )
    return;

  if ( !stringp(site) )
      error("bad site for banishing\n");

  LOG_D->log(LOG_BANISH,
	     sprintf("%s banished the site %s (%s) because: %s\n",
		     this_user()->query_userid(), site, 
		     ctime(time()), reason));

  bad_sites += ({ site });
  save_me();
}

void unbanish_site(string site)
{
  if ( !check_previous_privilege(1) )
    return;

  bad_sites -= ({ site });
  save_me();
}

int check_name(string name)
{
  // return 1 if banished

  return sizeof(filter_array(bad_names, (: regexp($(name), $1):) ));
}

int check_site()
{
  string *check;

  check = ({ query_ip_number(previous_object()), 
	     lower_case(query_ip_name(previous_object())) });

  return sizeof(filter
		(bad_sites, (: sizeof(regexp($(check), translate($1))) :)));
}

mixed *show_banishes()
{
  return copy(({ bad_names, bad_sites }));
}
