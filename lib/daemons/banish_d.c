/* Do not remove the headers from this file! see /USAGE for more info. */

// banish_d.c Jan 31, 1992 by Rust

#include <mudlib.h>

inherit DAEMON;

#define SAVE_PATH "/data/daemons/banish"

string* bad_names =({});
mixed* bad_sites = ({});

void banish_name(string name)
{
  if ( !check_previous_privilege(1) )
    return;

  if(!stringp(name)) return;
  bad_names += ({name});
  unguarded(1, (: save_object,SAVE_PATH :));
}

void unbanish_name(string name){
  if ( !check_previous_privilege(1) )
    return;

  bad_names -= ({name});
  unguarded(1, (: save_object,SAVE_PATH :));
}

void banish_site(mixed *site){
  if ( !check_previous_privilege(1) )
    return;

  if(!pointerp(site))return;
  bad_sites += ({ site });
  unguarded(1, (: save_object ,SAVE_PATH :));
}

void unbanish_site(string *site){
  if ( !check_previous_privilege(1) )
    return;

  bad_sites -= ({ site });
  unguarded(1, (: save_object,SAVE_PATH :));
}

int check_name(string name){
  // return 1 if banished
  return sizeof(filter_array(bad_names, (: $1[0] == '^' ? $(name) == $1[1..] : strsrch($(name), $1) != -1 :) ));
}

int check_site(string site){
  int i,j;
  mixed ipnum, check;
  int flag;

  i=sizeof(bad_sites);
  ipnum = explode(query_ip_number( previous_object() ), ".");
  while(i--){
    check = bad_sites[i];
    if(sizeof(check) != sizeof(ipnum)) continue;
    j = sizeof( check );
    while( j -- )
    {
	if( check[j] == "*" ) continue;
	if( check[j] != ipnum[j] )
	{
		flag = 1;
		break;
	}
    }
    if( !flag ) return 1;
    flag = 0;
  }
}

create(){
  ::create();
  restore_object(SAVE_PATH, 1);
}


mixed *show_banishes()
{
  return ({ bad_names, bad_sites });
}
