/* Do not remove the headers from this file! see /USAGE for more info. */

private nosave mapping exits = ([]);
private nosave string array hidden_exits = ({});
private nosave mapping exit_msg = ([]);
private nosave mapping enter_msg = ([ ]);
private nosave mapping checks = ([ ]);
private nosave mapping descriptions = ([ ]);
private nosave string def_desc = 0;

mixed eval_dest(mixed, string, string);
mixed query_exit_check(string);

string array standard_directions = ({ "up", "down", "north",
                                      "northwest", "northeast",
                                      "east", "west", "southeast",
                                      "south", "southwest" });

string array query_exit_directions(int show_hidden)
{
    string array dirs = keys( exits );

    if (show_hidden)
        return dirs;
    else
        return dirs - hidden_exits;
}

string array query_hidden_exits() {
    return hidden_exits;
}

mixed query_enter_msg(string arg)
{
    if (!objectp(exits[arg]))
       return evaluate(enter_msg[arg]);
    else
       return exits[arg]->query_enter_msg(arg);
}

mixed query_exit_msg(string arg)
{
    if (!objectp(exits[arg]))
        return evaluate(exit_msg[arg]);
    else
        return exits[arg]->query_exit_msg(arg);
}

void set_enter_msg(string dir, string msg)
{
    enter_msg[dir] = msg;
}

void set_exit_msg(string dir, mixed msg)
{
    exit_msg[dir] = msg;
}

mixed can_go_somewhere(string arg)
{
    return evaluate(query_exit_check(arg), arg, this_body());
}

mixed query_exit_destination(string arg, string base)
{
    return eval_dest(exits[arg], arg, base);
}

mixed eval_dest(mixed arg, string dir, string base)
{
    mixed tmp;
RABUG("eval_dest base: " + base);
    if (!objectp(arg))
        tmp = evaluate(arg);
    else
        tmp = arg->query_exit_destination(dir, base);
RABUG(sprintf("eval_dest eval'd arg: %O", tmp));
    if (!stringp(tmp) && !objectp(tmp)) return 0;
    if (objectp(tmp)) return tmp;
if (tmp[0] == '#') return tmp;
    if (tmp[0] != '/') {
        tmp = evaluate_path(base + tmp);
    }
RABUG(sprintf("eval_dest: %O", tmp));
    return tmp;
}

string query_exit_description(string arg)
{
    if (!objectp(exits[arg]))
        return evaluate(descriptions[arg]);
    else
        return exits[arg]->query_exit_description(arg);
}

mixed query_exit_check(string arg)
{ 
    if (!objectp(exits[arg]))
{
        return checks[arg];
}
    else
        return exits[arg]->query_exit_check(arg);
}

void set_exit_description(string dir, mixed desc)
{
    descriptions[dir] = desc;
}

void set_exit_check(string dir, function f)
{
    RABUG("Set_exit_check: " + dir);
    checks[dir] = f;
}

//:FUNCTION remove_hidden_exit
//Make a given exit direction no longer a hidden exit.  See set_hidden_exits
void remove_hidden_exit( string array exits_list ... )
{
    if( sizeof( exits_list ) == 1 && exits_list[0] == "all" )
        hidden_exits = 0;
    else
        hidden_exits -= exits_list;
}

//:FUNCTION delete_exit
//Remove a single exit from the room.  The direction should be an exit
//name.
void delete_exit(mixed direction)
{
    map_delete(exits, direction);
}
 
//:FUNCTION add_hidden_exit
//Make a given exit direction a hidden exit.  See set_hidden_exits
void add_hidden_exit( string array exits_list ... )
{
    if( sizeof( exits_list ) == 1 && exits_list[0] == "all" )
        hidden_exits = standard_directions;
    else
        hidden_exits += exits_list;
}

//:FUNCTION add_exit
//and the value should be a filename or a more complex structure as
//described in the exits doc.
varargs void add_exit(mixed direction, mixed value)
{
if (!objectp(value)) RABUG("add_exit: direction, value: "+direction+", "+value);
else { RABUG("add_exit: direction, value: "+direction+", "+file_name(value));
RABUG(format_list(query_exit_directions(1), ", "));
RABUG(file_name(this_object()));
}
    if (stringp(value) && strlen(value) == 0)
        error("Bogus exit passed into add_exit");
    if (stringp(value) && value[0] == '#')
        checks[direction] = value[1..];
    else
        checks[direction] = 1;
    exits[direction] = value;
    descriptions[direction] = def_desc;
    enter_msg[direction] = 0;
    exit_msg[direction] = 0;
RABUG(format_list(query_exit_directions(1), ""));
}


//:FUNCTION set_exits
//Sets the exit mapping of a room.  The keys should be exit names, the values
//should be either filenames or more complex structures described in the
//exits doc
void set_exits( mapping new_exits )
{
    mixed key;

    if ( mapp(new_exits) )
    {
        exits += new_exits;
        foreach (key in keys(new_exits)) {
            if (stringp(new_exits[key]) && new_exits[key][0] == '#')
                checks[key] = new_exits[key];
            else
                checks[key] = 1;
            descriptions[key] = def_desc;
            enter_msg[key] = 0;
            exit_msg[key]  = 0;
        }
    }
}

//:FUNCTION set_hidden_exits
//This is the list of exits to NOT be shown to the mortals in the room.
void set_hidden_exits( string array exits_list ... )
{
    if( sizeof( exits_list ) == 1 && exits_list[0] == "all" )
        hidden_exits = standard_directions;
    else
       hidden_exits = exits_list;
}




