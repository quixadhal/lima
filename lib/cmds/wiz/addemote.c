/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit M_INPUT;

private static string verb;
private static mixed rule;

mixed parse_rule(string str) {
    switch (str) {
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

private nomask void got_message(string str)
{
    string part2;

    sscanf(str, "%s && %s", str, part2);
    SOUL_D->add_emote(verb, rule, str, part2);
}

private nomask void got_rule(string str)
{
    rule = parse_rule(str);
    if ( rule == "." )
    {
        return;
    }

    write("Message: ");
    modal_simple((: got_message :));
}

nomask int main(string str) {
    rule = 0;
    if (!str) {
	write("addemote verb\n");
	return 0;
    }
    verb = str;
    sscanf(verb, "%s %s", verb, rule);
    if (!rule) {
	write("Rule: ");
	modal_simple((: got_rule :));
    } else {
        rule = parse_rule(rule);
	write("Message: ");
	modal_simple((: got_message :));
    }
    return 1;
}
