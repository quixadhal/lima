/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** openable.c (M_OPENABLE)
**
** This object provides base open/close capabilities for an OBJ
**
**
//### doc forthcoming...
*/

#include <hooks.h>
#include <flags.h>

/*
** From OBJ::description
*/
void set_in_room_desc(string arg);
string the_short();
varargs void add_adj();
varargs void remove_adj();

private string open_msg =  "$N $vopen a $o.";
private string close_msg = "$N $vclose a $o.";
private string open_desc;
private string closed_desc;

mixed call_hooks(string, int);
void add_hook(string, function);
void resync_visibility();
int test_flag(int which);
void assign_flag(int which, int state);

//:FUNCTION do_on_open()
// Called from open_with() so modules that inherit from M_OPENABLE
// Don't have to catch the "open" hook.
void do_on_open() {
    /* Overload this later */
}

//:FUNCTION do_on_close()
// Called from close() so modules that inherit from M_OPENABLE
// Don't have to catch the "close" hook.
void do_on_close() {
    /* Overload this later */
}

int openable() { return 1; }

int query_closed() { return !test_flag(F_OPEN); /* closed; */ }

void set_closed(int x)
{
    assign_flag(F_OPEN, !x);

    remove_adj("closed", "open");
    if (x)
	add_adj("closed");
    else
	add_adj("open");

    if (open_desc && !x) 
	set_in_room_desc(open_desc);
    if (closed_desc && x)
	set_in_room_desc(closed_desc);
  
    /* our inventory visibility probably just changed. */
    resync_visibility();
}


void set_open_desc( string desc )
{
  open_desc = desc;
  if (!query_closed()) set_in_room_desc(desc);
}

void set_closed_desc( string desc )
{
  closed_desc = desc;
  if (query_closed()) set_in_room_desc(desc);
}

string query_closed_desc() { return closed_desc; }

string query_open_desc() { return open_desc; }

void set_open_msg(string new_msg)
{
    open_msg = new_msg;
}

void set_close_msg(string new_msg)
{
    close_msg = new_msg;
}

varargs int open_with(object with)
{
    string ex;

    if (!query_closed())
    {
	write("It is already open.\n");
	return 1;
    }

//:HOOK prevent_open
//A yes/no/error hook which can prevent an object from being opened.

    ex = call_hooks("prevent_open", HOOK_YES_NO_ERROR);
    if (!ex) ex = capitalize(the_short() + " doesn't seem to want to open.\n");
    if (stringp(ex)) {
	write(ex);
	return 1;
    }
    
    this_body()->simple_action(open_msg, this_object());
    set_closed(0);
    if (ex = inv_list(all_inventory())) {
	write("Inside, you find:\n"+ex);
    }

    do_on_open();

//:HOOK open
//called when an object is opened.  The return value is ignored.
    call_hooks("open", HOOK_IGNORE);

    return 1;
}

mixed close() {
    mixed tmp;
    if (query_closed()) {
	write("It is already closed.\n");
	return 1;
    }
//:HOOK prevent_close
//A yes/no/error hook that can prevent an object from being closed
    tmp = call_hooks("prevent_close", HOOK_YES_NO_ERROR);
    if (!tmp) tmp = capitalize(the_short()) + " doesn't seem to want to close.\n";
    if (stringp(tmp)) return tmp;
    
    this_body()->simple_action(close_msg, this_object());
    do_on_close();

//:HOOK close
//called when an object is closed.  The return value is ignored.
    call_hooks("close", HOOK_IGNORE);

    set_closed(1);    
    return 1;
}

int is_open()
{
  return !query_closed();
}

/* Verb interaction */
mixed direct_open_obj(object ob) {
    object where = environment(this_object());

  if(where != this_body() && where != environment(this_body()) )
      return "#You do not have that.\n";
    if (!query_closed())
      return "It is already open.\n";
    return 1;
}

mixed direct_close_obj(object ob) {
    object where = environment(this_object());
    if (where != this_body() && where!= environment(this_body()) )
    return "#You do not have that.\n";
    if (query_closed())
        return "It is already closed.\n";
    return 1;
}


string extra_long_stuff()
{
    return capitalize(the_short()) + " is " + (query_closed() ? "closed" : "open") + ".\n";
}


// You should do this, or call set_closed() when you create an openable,
// so that the proper adjective gets initialized.
void internal_setup() {
    set_closed(1);
    
    add_hook("extra_long", (: extra_long_stuff :));
    add_hook("prevent_look_in", (: query_closed() ? "It is closed.\n" : (mixed)1 :));
}

mapping lpscript_attributes() {
    return ([ 
	"closed" : ({ LPSCRIPT_INT, "setup", "set_closed" }),
    ]);
}

// called once(?) when M_OPENABLE is first loaded.
#if 0
private void create()
{
    if ( !clonep() )
	M_OBJ_ATTRIBUTES->register_attribute(F_OPEN, ({ "closed", "open" }));
}
#endif
