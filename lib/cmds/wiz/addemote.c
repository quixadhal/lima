/* Do not remove the headers from this file! see /USAGE for more info. */
// out(f) isn't very appropriate here...


//:COMMAND
//$$ see: feelings, m_messages, rmemote, showemote, stupidemote, targetemote
//USAGE:  addemote (verb)
//
//This command allows you to add new souls.  (verb) being the soul.
//
//After entering the addemote soul you will be asked for the rule.
//The rule consists of one of the following; OBJ, LIV, STR etc.
//Then you will be asked for the message.
//
//addemote kick
//rule OBJ
//message: $N $vkick $T
//
//To add a new rule treat it as if the whole emote was new, just
//follow the same steps as for adding a brand new emote.
//
//The message can also be multiple messages separated by " && "; these
//as the message for the doer, others, the first target, the second target ...
//respectively.
//
//e.g. $N $vkick $t hard. && $N $vkick $t hard (how mean).
//
//gives:
//me: You kick Rust hard.
//rust: Beek kicks you hard.
//room: Beek kicks Rust hard (how mean).
//
//(when there is no message for the target, they see the first one)
//
//also: $N $vkick $t hard. && $N $vkick $t hard (how mean). && $N $vkick $t hard (ouch!).
//
//gives:
//me: You kick Rust hard.
//rust: Beek kicks you hard (ouch!).
//room: Beek kicks Rust hard (how mean).
//
//Extensive details of the messaging syntax are available in /contrib/emotehelp.txt

#include <daemons.h>
#include <log.h>

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
    string s;

    if ( str == "" )
    {
	write("Addemote aborted.\n");
	return;
    }

    str = replace_string(str, "\\n", "\n");
    parts = explode(str, " && ");
    SOUL_D->add_emote(verb, rule, parts);
    s = sprintf("%s added the emote %s at %s\n",
      this_user()->query_userid(),
      verb, ctime(time()));
    LOG_D->log(LOG_ADDEMOTE,s);
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
