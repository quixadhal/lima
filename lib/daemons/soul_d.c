/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <security.h>

inherit M_MESSAGES;
inherit M_COMPLETE;
inherit M_ACCESS;
inherit M_GRAMMAR;

private mapping emotes;
private string* adverbs;

#define SAVE_FILE "/data/daemons/soul"
#define CMD_ADD_EMOTE "/cmds/wiz/addemote"
#define CMD_REMOVE_EMOTE "/cmds/wiz/rmemote"
#define CMD_MOVE_EMOTE "/cmds/wiz/mvemote"

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


string stat_me()
{
    return "Number of feelings: " + sizeof(emotes) + "\n";
}

int
add_emote(string verb, mixed rule, string array parts)
{
/*
    if( base_name( previous_object()) != CMD_ADD_EMOTE )
	error("Illegal call to add_emote()\n");
*/
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
remove_emote(string verb, string rule) 
{
    if ( base_name(previous_object())!=CMD_REMOVE_EMOTE )
	error("Illegal call to remove_emote()\n");
    if (!emotes[verb]) return 0;
    if (sizeof( rule )) {
	if (!emotes[verb][rule]) return 0;
	map_delete(emotes[verb], rule);
    } else {
	map_delete(emotes, verb);
    }

    parse_remove(verb);
    parse_refresh();

    unguarded(1, (: save_object, SAVE_FILE :));
    return 1;
}

int
move_emote(string verb, string dest) 
{
    if (base_name(previous_object())!=CMD_MOVE_EMOTE)
	error("Illegal call to move_emote()\n");
    if ( (!emotes[verb]) || (!dest)) return 0;
    emotes[dest]=emotes[verb];
    foreach (string rule, string soul in emotes[verb])
    {
	parse_add_rule(dest, rule);
    }
    map_delete(emotes, verb);
    unguarded(1, (: save_object, SAVE_FILE :));
    return 1;
}

mixed
query_emote(string em) {
    return emotes[em];
}

private string get_completion(string);

#define IMUD_CHANNELT_DUMMY "/obj/mudlib/ichannelt"

mixed * internal_get_soul(string verb, string rule, mixed *args,
			  int add_imud_msg)
{
    mapping rules;
    mixed soul;
    int i, j, num;
    object *who, *imud_who;
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
    } else {
	soul = map_array(soul, (: $1[<1] == '\n' ? $1 : $1 + "\n" :));
    }


    who = ({ this_body() });
    if ( add_imud_msg )
      imud_who = ({ new( IMUD_CHANNELT_DUMMY, this_body(), 3 ) });

    if (strsrch(rule, "LIV") != -1)
    {
	i = 0;

	foreach (token in explode(rule, " ")) {
	    if (token == "LIV") {
		who += ({ args[i] });
		
		if ( add_imud_msg )
		  imud_who += ({ new( IMUD_CHANNELT_DUMMY, args[i], 2 ) });
		
		args[i..i] = ({});
	    } else
	      if (token[0] >= 'A' && token[0] <= 'Z')
		i++;
	}
    }
    
    if ( add_imud_msg )
      result = ({ who, allocate( sizeof( who )+1 ),
		  allocate( sizeof( imud_who )+1) });
    else
      result = ({ who, allocate(sizeof(who) + 1) });

    for ( j = 0; j < add_imud_msg+1; j++ )
    {
	object *w;

	w = ( j ? imud_who : who );
//RABUG(sprintf("%O", w));

	for (i = 0; i < sizeof(w); i++) {
	    string tmp;
	    if (stringp(soul))
	      tmp = soul;
	    else {
		if (i && i + 1 < sizeof(soul))
		  tmp = soul[i + 1];
		else
		  tmp = soul[0];
	    }
//RABUG(sprintf("i: %d, tmp: %s, soul: %O", i, tmp, soul));
//RABUG(sprintf("w[i]: %O, w: %O, args: %O", w[i], w, args));
	    result[1+j][i] = compose_message(w[i], tmp, w, args...);
	}
	//### tmp fix
	result[1+j][<1] =
	  compose_message(0, stringp(soul) ? soul : soul[1], w, args...);
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
	    if (arrayp(data)) {
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
	    write("Can't find a unique match.\nFound: " + implode(completions, ", ") + "\n");
	    return 0;
	}
}

/*
** Interface with parsing functions.  We use the "wild" card functions
** so that we don't have to support a gazillion can/do type actions.
*/
int livings_are_remote() { return 1; }

mixed can_verb_wrd(string verb, string wrd) 
{
    return member_array(wrd, adverbs) != -1 || member_array('*', wrd) != -1;
}

mixed can_verb_rule(string verb, string rule)
{
    if (!emotes[verb]) return;
    return !undefinedp(emotes[verb][rule]);
}

mixed direct_verb_rule(string verb, string rule) 
{
    return !undefinedp(emotes[verb][rule]);
}

mixed indirect_verb_rule(string verb, string rule) 
{
    return !undefinedp(emotes[verb][rule]);
}

void do_verb_rule(string verb, string rule, mixed args...)
{
    mixed soul;
    
//    RABUG(sprintf("do_verb_rule: (args: %O)", args));
    soul = get_soul(verb, rule, args);
    if (!soul) return;
    if ( sizeof(soul[0]) == 2 &&
	 !immediately_accessible(soul[0][1]))
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
    if( intp(result) || stringp(result)) return 0;
    soul = get_soul(result[0], result[1], result[2..]);
    if (!soul) return 0;
    return soul;
}

mixed *parse_imud_soul(string str) {
    object temp;
    mixed result;
    mixed soul;
    int i;

    // This bit of the code can only handle one person,
    // could make it handle more, but its not worth it while the IMUD stuff
    // only handles two people
    i = strsrch( str, "@" );
    if ( i > -1 ) 
    {
	string n;
	n = str[ strsrch( str[0..i], " ", -1 )+1..
		strsrch( str[i..]+" ", " " )-1+i ];
	temp = new( IMUD_CHANNELT_DUMMY, n );
	if ( temp )
	  str = replace_string( str, n, "wibblewibble"+n );
    }
    
    result = parse_my_rules(this_body(), str, 0 );

    if (!result || intp(result) || stringp(result))
    {
	if ( temp ) temp->remove();
	return 0;
    }

    soul = get_imud_soul(result...);

    if (!soul)
      return 0;
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


   
