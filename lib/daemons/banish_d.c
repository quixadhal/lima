/* Do not remove the headers from this file! see /USAGE for more info. */

// banish_d.c Jan 31, 1992 by Rust  // Uh?  Don't think I was mudding then...
//  Try Jan 31, 1993.


inherit DAEMON;
inherit M_GLOB;

#define SAVE_PATH "/data/daemons/banish"

string* bad_names =({});
mixed* bad_sites = ({});

void banish_name(string name, string reason)
{
  if ( !check_previous_privilege(1) )
    return;

  if(!stringp(name)) return;

#ifdef BANISH_LOG
	unguarded(1, (:write_file, BANISH_LOG, 
		      sprintf("%s banished the name %s (%s) because: %s\n",
	  this_user()->query_userid(), name, ctime(time()), reason) :));
#endif

  bad_names += ({name});
  unguarded(1, (: save_object,SAVE_PATH :));
}

void unbanish_name(string name)
{
  if ( !check_previous_privilege(1) )
    return;

  bad_names -= ({name});
  unguarded(1, (: save_object,SAVE_PATH :));
}

void banish_site(string site, string reason)
{
  if ( !check_previous_privilege(1) )
    return;
#ifdef BANISH_LOG
	unguarded(1, (:write_file, BANISH_LOG, 
		      sprintf("%s banished the site %s (%s) because: %s\n",
			      this_user()->query_userid(), site, 
			      ctime(time()), reason) :));
#endif

  bad_sites += ({ site });
  unguarded(1, (: save_object ,SAVE_PATH :));
}

void unbanish_site(string site)
{
  if ( !check_previous_privilege(1) )
    return;

  bad_sites -= ({ site });
  unguarded(1, (: save_object,SAVE_PATH :));
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

create()
{
  daemon::create();
  restore_object(SAVE_PATH, 1);
}


mixed *show_banishes()
{
  return ({ bad_names, bad_sites });
}
