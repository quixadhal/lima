/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** openable.c (M_OPENABLE)
**
** This object provides base open/close capabilities for an OBJ
**
** ### doc forthcoming...
*/

/* ### this probably shouldn't be here... but for now (simul conversion)... */
#include <mudlib.h>


/*
** From OBJ::description
*/
void set_in_room_desc(string arg);
string the_short();
string* query_adj();
void set_adj(string*);


private int closed;
private string open_msg = "$N $vopen a $o.\n";
private string close_msg = "$N $vclose a $o.\n";
private string open_desc;
private string closed_desc;

int openable() { return 1; }
int query_closed() { return closed; }
void set_closed(int x) { 
  string* adjs;
  
  closed = x; 
  adjs = query_adj();
  if(!arrayp(adjs))
    adjs = ({});
  else adjs -= ({"closed","open"});
  set_adj(adjs + (string *)(closed ? ({"closed"}) : ({"open"})));
  parse_refresh(); 
}


void extra_open() { }
void extra_close() { }
int prevent_open() { return 0; }

void set_open_desc( string desc )
{
  open_desc = desc;
}

void set_closed_desc( string desc )
{
  closed_desc = desc;
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

int open_with(object with)
{
    string ex;

    if (!query_closed())
    {
	write("It is already open.\n");
	return 1;
    }
    if ( prevent_open() )
	return 1;

    this_body()->simple_action(open_msg, this_object());
    if (ex = inv_list(this_object(), 0, 1)) {
	write("Inside, you find:\n"+ex);
    }
    extra_open();
    if( open_desc )
	set_in_room_desc( open_desc );
    set_closed(0);
    return 1;
}

int close() {
    if (query_closed()) {
	write("It is already closed.\n");
	return 1;
    }
    if (prevent_open()) return 1;
    this_body()->simple_action(close_msg, this_object());
    extra_close();
    if( closed_desc )
	set_in_room_desc( closed_desc );
    set_closed(1);    
    return 1;
}

int is_open()
{
  return !query_closed();
}

string
extra_long()
{
    return capitalize(the_short()) + " is " +
	(query_closed() ? "closed" : "open") + ".\n";
}

string
extra_short()
{
  return query_closed() ? 0 : "open";
}

/* Verb interaction */
mixed direct_open_obj(object ob) {
    if (!query_closed())
      return "It is already open.\n";
    return 1;
}

mixed direct_close_obj(object ob) {
    if (query_closed())
        return "It is already closed.\n";
    return 1;
}

// You should do this, or call set_closed() when you create an openable,
// so that the proper adjective gets initialized.
void create(){
  if ( !clonep() )
    return;

  set_closed(1);
}
