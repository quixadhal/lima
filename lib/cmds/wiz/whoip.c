/* Do not remove the headers from this file! see /USAGE for more info. */

  //  Yaynu@Lima Bean 950723
  //  Prints out a list of IP connections.
  //  Arguments: <none> - Reports all players IP names+numbers.
  //             <ipname,ipnum> - Reports all who are from that address.
  //             <name> - Reports out players ip name and number.
 
#include <mudlib.h>
#include <playerflags.h>
#define DIVIDER \
"-------------------------------------------------------------------------\n"

inherit CMD;
 
private void main(string arg) {
int x,cnt;
object *p,ob;
string *n, ip, str;                                               
  if(!arg){                                     
    printf("%s:  (Local Time is: %s)\n%s",
                  mud_name(), ctime(time()), DIVIDER);
    write("Name         IP number                 IP name\n"+DIVIDER);
    for(p = users(),x=sizeof(p),cnt=0;cnt<x;++cnt){
        printf("%-12s %-25s %-25s\n",capitalize(p[cnt]->query_real_name()),
        query_ip_number(p[cnt]), query_ip_name(p[cnt]));
    }                                      
    printf("%sCurrently there %s %d player%s.\n",DIVIDER,
          (x>1?"are":"is"),x,(x>1?"s":""));
    return;
  } else if (ob = find_user(arg)) {
    ip = query_ip_number(ob); 
    p = filter(users(), (: query_ip_number($1) == $2 :), ip);
  } else if (sscanf(implode(explode(arg,"."),""),"%d",x) == 1){
    p = filter(users(), (: query_ip_number($1) == $2 :), arg);  
  } 
    else if (sscanf(implode(explode(arg,"."),""),"%s",x) == 1){
    p = filter(users(), (: query_ip_name($1) == $2 :), arg);  
  } 
    else {             
    arg = lower_case(arg);
    p = filter(users(), (: query_ip_number($1) == $2 :), arg);  
  } 
  if (x=sizeof(p)) {
    n = map(p, (: capitalize($1->query_real_name()) :)); 
    str = query_ip_name(p[0]);
    if (str != query_ip_number(p[0]))  str += " ("+query_ip_number(p[0])+")";
      printf("There %s %d user%s from %s:\n",
      (x>1?"are":"is"),x,(x>1?"s":""),str);  
      printf("%-=78s",implode(n,", ")+".\n");
   } else write("Nobody is connected from that address.\n");
   return;
 }  
 
