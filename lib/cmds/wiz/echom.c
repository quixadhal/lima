/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;
inherit M_GRAMMAR;

private void main(string args) {
    string who;
    object targ;
    
    if (!sizeof(args)) {
	write("Act how?\n");
	return;
    }

    sscanf(args, "@%s %s", who, args);
    args = punctuate(args);
    
    if (who && (targ = find_body(who))) {
	this_body()->targetted_action(args, targ);
    } else {
	this_body()->simple_action(args);
    }
}
