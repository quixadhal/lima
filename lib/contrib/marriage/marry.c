/* Do not remove the headers from this file! see /USAGE for more info. */

// Function to marry two people. Both must be online.
// Code by Aziz
#include <mudlib.h>
#include <daemons.h>

inherit CMD;

private void main(mixed victims){
object v1, v2;
string victim1, victim2;

string *vic=explode(victims, " ");

if(!victims){
write("Usage: marry victim1 victim2\n");
return;
}
victim1 = vic[0];      
victim2 = vic[1];

if(!v1=find_body(victim1)){
write("Unable to find "+capitalize(victim1)+", aborting.\n");
return;
}

if(!v2=find_body(victim2)){
write("Unable to find "+capitalize(victim2)+", aborting.\n");
return;
}

v1->set_spouse(victim2);
v2->set_spouse(victim1);
write("Ok. You've just married "+capitalize(victim1)+" and "+capitalize(victim2)+".\n");
CHANNEL_D->deliver_emote("announce", this_body()->query_name(),
sprintf("has just married "+capitalize(victim1)+" and "+capitalize(victim2)+"!!!
Rejoice!"));
this_body()->simple_action("$N has just married "+capitalize(victim1)+" and "+capitalize(victim2)+"!");
return;
}
