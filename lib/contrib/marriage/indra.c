/* Do not remove the headers from this file! see /USAGE for more info. */

inherit LIVING;
inherit M_TRIGGERS;
inherit M_ACTIONS;

void setup(){
set_name("Indra");
set_gender(2);
set_proper_name("Indra");
set_in_room_desc("An elven wench named Indra is here.");
set_long("Indra is a dazzlingly beautiful elven woman. She looks at you"+
" with a smile that almost melts you where you stand. And taking into account"+
" that she is also stark naked, you don't have much to say right now.");
add_pattern("%s smiles.", (: $1 !="Indra Elvenmist" ? "emote smiles happily"+
" at you." : 0 :));
add_pattern("%s kisses%s", "emote returns your kiss and puts her arms around"+
" your neck.");
add_pattern("%s pulls %s", "emote lets her tounge play around in your"+
" mouth, stroking her hands through your hair.");
add_pattern("%s says, \"Fuck me gently with a chain saw!\"", "emote starts up"+
" her chainsaw and grins happily at you.");

}
