/* Do not remove the headers from this file! see /USAGE for more info. */

 
inherit M_EXIT;

#include <hooks.h>

varargs mixed call_hooks(string tag, mixed func, mixed start,
                         array args...);
string get_default_exit();
void do_go_somewhere(string);

private nosave string base;
private nosave mixed def_exit;

void create() {
    base = file_name(this_object());
    base = base[0..strsrch(base, '/', -1)];
}

string query_base() {
    return base;
}

mixed can_go_somewhere(string dir) {
    mixed value;
    value = ::can_go_somewhere(dir);
    if (!stringp(value) && value !=1 && is_normal_direction(dir))
       return get_default_exit();
    return value;
}


//:FUNCTION set_default_exit
//Set the default exit message (the message given when someone goes a direction
//with no exit).  This should be a string or a function ptr returning a string.
void set_default_exit(mixed value) {
    def_exit = value;
}

//:FUNCTION has_default_exit
//returns true if the room has a default exit error
int has_default_exit() {
    return !!def_exit;
}

string get_default_exit()
{
    if (has_default_exit())
        return evaluate(def_exit);
    return( "It doesn't appear possible to go that way.\n");
}

void do_go_obj(object ob, mixed s) {
    if (stringp(s) && (member_array(s, query_exit_directions(1)) != -1 )) {
        do_go_somewhere(s);
        return;
    }
    if (call_hooks("block_all", HOOK_LOR, 0, 0))
        return;
    if (!stringp(s))
        this_body()->move_to(ob->query_default_destination(),
                             0,
                             ob->query_default_exit_msg(),
                             ob->query_default_enter_msg());
    else
        this_body()->move_to(ob->query_prep_destination(s),
                             s,
                             ob->query_prep_exit_msg(s),
                             ob->query_prep_enter_msg(s));
}

void do_go_somewhere(string dir) {
    if (call_hooks("block_" + dir, HOOK_LOR, 0, dir)
      || call_hooks("block_all", HOOK_LOR, 0, dir))
        return;  
    this_body()->move_to(query_exit_destination(dir, base), 
                         dir,
                         query_exit_msg(dir),
                         query_enter_msg(dir));
}



mixed query_exit_destination(string arg) {
    mixed tmp = ::query_exit_destination(arg, base);
    if (!tmp) return get_default_exit();
    return tmp;
}

//:FUNCTION show_exits
//Return a string giving the names of exits for the obvious exits line
string show_exits()
{
    string exit_str;
    string* exit_names;

    exit_names = query_exit_directions(0);
    exit_str = ((sizeof(exit_names)) ? implode(exit_names,", ") : "none");

#ifdef WIZARDS_SEE_HIDDEN_EXITS
    if ( wizardp(this_user()) && sizeof(hidden_exits) )
    {
	exit_str += ", *" + implode(hidden_exits, ", *");
    }
#endif

    return exit_str;
}

