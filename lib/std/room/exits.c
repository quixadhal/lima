/* Do not remove the headers from this file! see /USAGE for more info. */

private static string base;
private static mixed def_exit;
private static mapping exits = ([]);
private static string array hidden_exits = ({});
private static mapping exit_msg = ([]);
private static mapping enter_msg = ([ ]);

void create() {
    base = file_name(this_object());
    base = base[0..strsrch(base, '/', -1)];
}

string array query_exit_directions(int show_hidden) {
    if (show_hidden)
	return keys(exits);
    else
	return keys(exits) - hidden_exits;
}

//:FUNCTION set_default_exit
//Set the default exit message (the message given when someone goes a direction
//with no exit).  This should be a string or a function ptr returning a string.
void set_default_exit(mixed value) {
    def_exit = value;
}

mixed query_exit_value(string direction, int handle_default) {
    mixed tmp = evaluate(exits[direction]);
    
    if (!tmp && def_exit && handle_default) return "#" + evaluate(def_exit);
    if (objectp(tmp)) return file_name(tmp);
    if (!stringp(tmp)) return 0;
    if (tmp[0] == '#') return tmp;
    if (tmp[0] != '/') {
	if (tmp == exits[direction]) {
	    //expand so we don't have to expand this again; but we can't
	    //do this trick if the wierd string was returned by a function,
	    //hence the above check.
	    exits[direction] = evaluate_path(base + tmp);
            return exits[direction];
	}
	return evaluate_path(base + tmp);
    }
    return tmp;
}

//:FUNCTION has_default_exit
//returns true if the room has a default exit error
int has_default_exit() {
    return !!def_exit;
}

//:FUNCTION show_exits
//Return a string giving the names of exits for the obvious exits line
string show_exits()
{
    string exit_str;
    string* exit_names;

    exit_names = keys(exits) - hidden_exits;
    exit_str = ((sizeof(exit_names)) ? implode(exit_names,", ") : "none");

#ifdef WIZARDS_SEE_HIDDEN_EXITS
    if ( wizardp(this_user()) && sizeof(hidden_exits) )
    {
       exit_str += ", *" + implode(hidden_exits, ", *");
    }
#endif

    return exit_str;
}

//### maybe have a better interface than global vars
string query_enter_msg(string arg)
{
    return enter_msg[arg]; 
}

string query_exit_msg(string arg)
{
    return exit_msg[arg]; 
}

void set_enter_msg(string dir, string msg)
{
  enter_msg[dir] = msg;
}

void set_exit_msg(string dir, string msg)
{
  exit_msg[dir] = msg;
}

//:FUNCTION add_exit
//Adds a single exit to the room.  The direction should be an exit name,
//and the value should be a filename or a more complex structure as
//described in the exits doc.
void add_exit(mixed direction, mixed value)
{
    exits[direction] = value;
}

//:FUNCTION delete_exit
//Remove a single exit from the room.  The direction should be an exit
//name.
void delete_exit(mixed direction)
{
    map_delete(exits, direction);
}

//:FUNCTION set_exits
//Sets the exit mapping of a room.  The keys should be exit names, the values
//should be either filenames or more complex structures described in the
//exits doc
void set_exits( mapping new_exits )
{
    if ( mapp(new_exits) )
	exits = new_exits;
}

//:FUNCTION set_hidden_exits
//This is the list of exits to NOT be shown to the mortals in the room.
void set_hidden_exits( string array exits_list ... )
{
  hidden_exits = exits_list;
}

