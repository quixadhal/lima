/* Do not remove the headers from this file! see /USAGE for more info. */

/* Created by Darklord */
/* April 1997 */

inherit CMD;

private void main(string arg)
{
    if(!this_body()) return;
    if(!arg)
	arg = "all";
    foreach(object ob in all_inventory(environment(this_body()))) {
	if(ob->query_vendor()) {
	    ob->query_items(arg, 0);
	    return;
	}
    }
    out("Sorry, there are no vendors in this room.\n");
}
