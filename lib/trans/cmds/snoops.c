/* Do not remove the headers from this file! see /USAGE for more info. */

// This command works only for admins.  -- Rust

#include <mudlib.h>

inherit DAEMON;

int main()
{
    object ob;
    int flag;
    int i;

    flag = 0;
    if(check_previous_privilege(1))
      for(i=0;i<sizeof(users());i++){
	ob = unguarded(1, (:query_snooping(users()[$(i)]):));
	if(ob && ob != 0){
	  printf("%s is snooping %s.\n", 
		 capitalize(users()[i]->query_real_name()),
		 capitalize(ob->query_real_name()));
	  flag = 1;
	}
      }
    if(flag == 0) write("Nobody is being snooped.\n");
    return 1;
}
