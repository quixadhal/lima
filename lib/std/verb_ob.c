/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** verb_ob.c
**
** This is a standard verb handler.  It handles adding and removing
** verb rules (to/from the parser efuns).
*/

#include <mudlib.h>

/*
** query_verb_info()
**
** This is the heart of this object.  Subclasses should override and
** return an array with 1 or 2 elements.  The first element should
** be an array of rules.  If the second is provided, then it should
** be an array of synonyms.
*/
mixed * query_verb_info()
{
    return ({ ({ }) });
}

void create()
{
    mixed *	info;
    string	verb;

    parse_init();
    verb = split_path(file_name())[1];
    info = query_verb_info();

    // backwards compat
    info[0] = map(info[0], (: (stringp($1) ? $1 : ($1 ? "STR" : "") ) :) );
    // unsupported
    info[0] = filter(info[0], (: strsrch($1,"OBS") == -1 :));
    
    if ( !info || !pointerp(info) || !pointerp(info[0]) )
	throw("bad verb info");

    map_array(info[0], (: parse_add_rule($(verb), $1) :));

    if (sizeof(info) == 2)
      map_array(info[1], (:parse_add_synonym($1, $(verb)):));

/* Old stuff         foreach (verb in info[1])
            map_array(info[0], (: parse_add_rule($(verb), $1) :));
*/
}

string refer_to_object(object ob) {
    // In the future, this should be smarter.  Ideally it would generate
    // something unique like 'my first sword' or 'the third sword on the table'
    return ob->query_id()[0];
}
 
mixed try_to_acquire(object ob) {
    /* for things that can be used in certain conditions without
     * actually being in the player's inventory
     */
    if (ob->always_usable()) return 1;
    
    if (environment(ob) == this_body()) return 1;
    write("(Taking " + ob->the_short());
    if (environment(ob) != environment(this_body()))
	write(" from " + environment(ob)->the_short());
    write(" first)\n");
    this_body()->do_game_command("get " + refer_to_object(ob));
    return environment(ob) == this_body();
}     

mixed check_ghost() {
    if (this_body()->query_ghost())
	return "But you're a ghost!\n";
    return 1;
}

mixed check_vision() {
    if (environment(this_body())->query_light())
        return 1;
    return "You can't see a thing!\n";
}

mixed check_condition() {
    mixed tmp;
    
    if (tmp = this_body()->check_condition(0))
	return tmp;
    return 1;
}

/* All (most) can_* functions should call this */
mixed default_checks() {
    mixed tmp;

    if ((tmp = check_vision()) != 1)
	return tmp;
    if ((tmp = check_ghost()) != 1)
	return tmp;

    return check_condition();
}

/* we defined the rule, so assume by default we allow it */
mixed can_verb_rule(string verb, string rule) {
    return default_checks();
}
