/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_GRAMMAR;


int main(string s)
{
    object ob;
    string who, msg;
    if (!s || !sscanf(s, "%s %s", who, msg))
	return printf("Usage: whisper [person] [message]\n"),1;
    if (!ob = present(who, environment(this_body())))
	return printf("No player with that name in this room.\n"),1;
    if (!ob->is_living())
	return printf("But %s is not alive! (and probably not interested)\n", who),1;
    msg = punctuate(msg);
    tell_object(ob, sprintf("%s whispers to you: %s\n", 
			    this_body()->query_name(), msg));
    printf("You whisper to %s: %s\n", capitalize(who), msg);
    say(sprintf("%s whispers something to %s.\n", this_body()->query_name(),
		capitalize(who)));
    return 1;
}
