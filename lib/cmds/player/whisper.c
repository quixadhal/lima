/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_GRAMMAR;


private void main(string s)
{
    object ob;
    string who, msg;
    if (!s || !sscanf(s, "%s %s", who, msg)) {
      printf("Usage: whisper [person] [message]\n");
      return;
    }
    if (!ob = present(who, environment(this_body()))) {
      printf("No player with that name in this room.\n");
      return;
    }
    if (!ob->is_living()) {
      printf("But %s is not alive! (and probably not interested)\n", who);
      return;
    }
    msg = punctuate(msg);
    tell_object(ob, sprintf("%s whispers to you: %s\n", 
			    this_body()->query_name(), msg));
    printf("You whisper to %s: %s\n", capitalize(who), msg);
    say(sprintf("%s whispers something to %s.\n", this_body()->query_name(),
		capitalize(who)));
    return;
}
