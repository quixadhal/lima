/* Do not remove the headers from this file! see /USAGE for more info. */
// out(f) isn't very appropriate here...


#include <mudlib.h>
#include <daemons.h>

inherit M_INPUT;
inherit CMD;

mixed parse_rule(string str)
{
    switch (str)
    {
    case "":
    case 0:
    case "0":
	return "";

    case "1":
    case "all":
	return "STR";

    default:
	return str;
    }
}

private nomask void got_message(string verb, string rule, string str)
{
    string array parts;

    if ( str == "" )
    {
	write("Addemote aborted.\n");
	return;
    }

    str = replace_string(str, "\\n", "\n");
    parts = explode(str, " && ");
    SOUL_D->add_emote(verb, rule, parts);
}

private nomask void got_rule(string verb, string str)
{
    string rule = parse_rule(str);

    if ( rule == "." )
    {
	write("Addemote aborted.\n");
	return;
    }

    write("Separate message rules for different message targets by '&&'s.\n");
    write("e.g.: This is seen by the user && This is seen by the target && This is seen by everyone else\n");
    write("Message: ");
    modal_simple((: got_message, verb, rule :));
}

nomask private void main(string str)
{
    string rule = 0;
    string verb = str;

    if (!str)
    {
	write("addemote verb\n");
	return;
    }

    sscanf(verb, "%s %s", verb, rule);
    if ( !rule )
    {
	write("(type '.' to abort entry)\nRule: ");
	modal_simple((: got_rule, verb :));
    }
    else
    {
	rule = parse_rule(rule);
	write("Message: ");
	modal_simple((: got_message, verb, rule :));
    }
}
