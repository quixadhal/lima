/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <security.h>

inherit M_MESSAGES;
inherit M_COMPLETE;
inherit M_ACCESS;

private mapping emotes;
private string* adverbs;

#define SAVE_FILE "/data/daemons/soul"

void
create() {
    string verb, rule;
    mapping value;
    mixed ignore;
    
    unguarded(1, (: restore_object, SAVE_FILE, 1 :));

    parse_init();

    foreach (verb, value in emotes) {
	if (value[0]) {
	    value[""] = value[0];
	    map_delete(value, 0);
	}
	if (value[1]) {
	    value["STR"] = value[1];
	    map_delete(value, 1);
	}
	
	foreach (rule, ignore in value) {
	    catch(parse_add_rule(verb, rule));
	}
    }

    if (!adverbs)
        adverbs = ({});
}


int stat_me() {
    write("Number of emotes: " + sizeof(emotes));
    return 1;
}

int
add_emote(string verb, mixed rule, string array parts)
{
    // try this first b/c it will error if rule is illegal
    parse_add_rule(verb, rule);

    if (!emotes[verb])
	emotes[verb] = ([]);

    if (sizeof(parts) > 1)
	emotes[verb][rule] = parts;
    else
	emotes[verb][rule] = parts[0];

    unguarded(1, (: save_object, SAVE_FILE :));

    return 1;
}

int
remove_emote(string verb, string rule) {
    if (!emotes[verb]) return 0;
    if (rule) {
	if (!emotes[verb][rule]) return 0;
	map_delete(emotes[verb], rule);
    } else {
	map_delete(emotes, verb);
    }

    unguarded(1, (: save_object, SAVE_FILE :));
    return 1;
}

mixed
query_emote(string em) {
    return emotes[em];
}

private string get_completion(string);

mixed * internal_get_soul(string verb, string rule,
			  mixed *args,
			  int add_imud_msg)
{
    mapping rules;
    mixed soul;
    string soul_alt;
    int i, idx, num;
    object *targets;
    object *who;
    string token;
    mixed * result;

    rules = emotes[verb];
    if (!rules) return 0;
    soul = rules[rule];
    if (!soul) return 0;

    // minus the verb's real name; we don't want to process the real names
    // of any of the objects
    num = (sizeof(args) - 1)/2;
    
    for (i = 0; i < num; i++) {
	if (stringp(args[i]) && strlen(args[i]) && args[i][<1] == '*'
	    && member_array(' ', args[i]) == -1) {
	    args[i] = get_completion(args[i][0..<2]);
	    if (!args[i])
		return 0;
	    break;
	}
    }

    if (soul[0] == '=') soul = rules[soul[1..]];
    if (stringp(soul)) {
	if (soul[<1] != '\n') soul += "\n";
	soul_alt = soul;
    } else {
	soul = map_array(soul, (: $1[<1] == '\n' ? $1 : $1 + "\n" :));
	soul_alt = soul[0];
    }
    if (strsrch(rule, "LIV") == -1) {
	who = ({ this_body() });
	result = ({ who, ({
	    compose_message(this_body(), stringp(soul) ? soul : soul[0], who,
			    args...),
	    compose_message(0, stringp(soul) ? soul : soul[1], who, args...)
	}) });
    } else {
	i = 0;
	targets = ({ });
	foreach (token in explode(rule, " ")) {
	    if (token == "LIV") {
		targets += ({ args[i] });
		args[i..i] = ({});
	    } else
	    if (token[0] >= 'A' && token[0] <= 'Z')
		i++;
	}

	who = ({ this_body() }) + targets;
	result = ({ who, allocate(sizeof(who) + 1) });
	for (i = 0; i < sizeof(who); i++) {
	    string tmp;
	    if (stringp(soul))
		tmp = soul;
	    else {
		if (i && i + 1 < sizeof(soul))
		    tmp = soul[i + 1];
		else
		    tmp = soul[0];
	    }
	    result[1][i] = compose_message(who[i], tmp, who, args...);
	}
	result[1][<1] = compose_message(0, stringp(soul) ? soul : soul[1],
					who, args...);
    }

    if ( add_imud_msg )
    {
	string msg;

	soul_alt = replace_string(soul_alt, "$N", chr(255));
	soul_alt = replace_string(soul_alt, "$n", chr(255));
	msg = compose_message(0, soul_alt, who, args...);
	msg = replace_string(msg, chr(255), "$N");
	result[1] += ({ msg });
    }

    return result;
}

varargs mixed *
get_soul(string verb, string rule, mixed *args)
{
    return internal_get_soul(verb, rule, args, 0);
}

varargs mixed *
get_imud_soul(string verb, string rule, mixed args...)
{
    return internal_get_soul(verb, rule, args, 1);
}

		    
mixed
list_emotes()
{
    return keys( emotes );
}

string *
emote_apropos(string str) {
    int num_verbs, num_rules;
    int i, j;
    string *verbs, *rules;
    mapping rules_for_verb;
    mixed data;
    string *found;

    found = ({});
    verbs = keys(emotes);
    num_verbs = sizeof(verbs);
    for (i=0; i<num_verbs; i++) {
	rules_for_verb = emotes[verbs[i]];
	rules = keys(rules_for_verb);
	num_rules = sizeof(rules);
	for (j=0; j<num_rules; j++) {
	    data = rules_for_verb[rules[j]];
	    if (pointerp(data)) {
                if (strsrch(lower_case(data[0]), lower_case(str)) != -1 || 
		    strsrch(lower_case(data[1]), lower_case(str)) != -1)
		    found += ({ verbs[i] + " " + rules[j] });
	    }
	    else {
                if (strsrch(lower_case(data), lower_case(str)) != -1)
		    found += ({ verbs[i] + " " + rules[j] });
	    }
	}
    }
    return found;
}


private string get_completion(string s)
{
    string * completions;
    
    completions = complete(s, adverbs);
    switch(sizeof(completions))
    {
      case 0:
	write("Can't find a match for '" + s + "*'.\n");
        return 0;
      case 1:
	return completions[0];
      default:
	write(wrap("Can't find a unique match.\nFound: " + implode(completions, ", ") + "\n"));
	return 0;
    }
}

/*
** Interface with parsing functions.  We use the "wild" card functions
** so that we don't have to support a gazillion can/do type actions.
*/
int livings_are_remote() { return 1; }

mixed can_verb_wrd(string verb, string wrd) {
    return member_array(wrd, adverbs) != -1 || member_array('*', wrd) != -1;
}
    
mixed can_verb_rule(string verb, string rule)
{
    return !undefinedp(emotes[verb][rule]);
}

mixed direct_verb_rule(string verb, string rule) {
    return !undefinedp(emotes[verb][rule]);
}

mixed indirect_verb_rule(string verb, string rule) {
    return !undefinedp(emotes[verb][rule]);
}

void do_verb_rule(string verb, string rule, mixed args...)
{
    mixed soul;
    
    soul = get_soul(verb, rule, args);
    if (!soul) return;
    if ( sizeof(soul[0]) == 2 &&
	!immediatly_accessible(soul[0][1]))
    {
	soul[1][0] = "*" + soul[1][0];
	soul[1][1] = "*" + soul[1][1];
	inform(soul[0], soul[1], 0);
    }
    else
    {
	inform(soul[0], soul[1], environment(this_body()));
    }

    return; 
}

mixed *parse_soul(string str) {
    mixed result;
    mixed soul;

    result = parse_my_rules(this_body(), str, 0);
    if (!result) return 0;
    soul = get_soul(result[0], result[1], result[2..]);
    if (!soul) return 0;
    return soul;
}

mixed *parse_imud_soul(string str) {
    mixed result;
    mixed soul;

    result = parse_my_rules(this_body(), str, 0);
    if (!result) return 0;
    soul = get_imud_soul(result...);
    if (!soul) return 0;
    return soul;
}

void set_adverbs(string* mods)
{
    if(!arrayp(mods)) error("bad arg type");
    adverbs = mods;
    unguarded(1, (: save_object, SAVE_FILE :));
}


string* get_adverbs(){
    return adverbs;
}

void add_adverb(string adverb)
{
  if(!stringp(adverb)) error("bad arg type");
  adverbs += ({adverb});
  unguarded(1, (: save_object, SAVE_FILE :));
}

void remove_adverb(string adverb)
{
  adverbs -= ({adverb});
  unguarded(1, (: save_object, SAVE_FILE :));
}


int total_number_of_rules()
{
  string verb;
  mapping value;
  int i;

  foreach(verb, value in emotes){
    i+= sizeof(value);
  }
return i;
}
