/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;
inherit M_MESSAGES;
inherit M_GRAMMAR;

void whisper_it(object liv, string str) {
    string *msgs;
    object *who = ({ this_body(), liv });

    str = punctuate(str);

    msgs = action(who, "$N $vwhisper to $t: $o\n", str);
    // change other message
    msgs[2] = this_body()->query_name() + " whispers something to " 
	      + liv->query_name() + ".\n";
    inform(who, msgs, environment(this_body()));
}
    
void do_whisper_liv_str(object liv, string str) {
    whisper_it(liv,str);
}

void do_whisper_str_to_liv(string str, object liv) {
    whisper_it(liv,str);
}

void do_whisper_to_liv_str(object liv, string str) {
    whisper_it(liv,str);
}

array query_verb_info()
{
    return ({ ({ "LIV STR", "STR to LIV", "to LIV STR" }) });
}

       
	
	
