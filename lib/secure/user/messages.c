/* Do not remove the headers from this file! see /USAGE for more info. */

/* %^RESET%^ gets used early in the login sequence by tell() */
private nosave mapping translations = (["RESET" : ""]);
#ifdef CONFIGURABLE_COLOUR
private mapping colours;
#endif

void save_me();
object query_shell_ob();

int screen_width;

void set_screen_width( int width )
{
    screen_width = width;
    this_user()->save_me();
}

int query_screen_width()
{
    return screen_width ? screen_width : 79;
}

void update_translations() {
#ifdef CONFIGURABLE_COLOUR
    /* handle new people, old save files, or save files for which this
     * option was disabled */
    if (!colours) colours = ([]);
    /* defaults; we take advantage of the fact that mapping addition
     * chooses the second when there is an overlap.
     */
    colours = ANSI_D->defaults() + colours;
#endif
    if (query_shell_ob()->get_variable("ansi"))
	translations = ANSI_D->query_translations()[0];
    else
	translations = ANSI_D->query_translations()[1];
#ifdef CONFIGURABLE_COLOUR
    translations = copy(translations);
    foreach (string code, string value in colours) {
	string array parts = map(explode(value, ","), (: upper_case :));
	string val = "";
	
	foreach (string item in parts) {
	    if (translations[item])
		val += translations[item];
	}
	translations[code] = val;
    }
#endif
}

#ifdef CONFIGURABLE_COLOUR
void set_colour(string which, string what) {
    colours[upper_case(which)] = what;
    update_translations();
    save_me();
}

void query_colour(string which) {
    return colours[which];
}

array query_colours() {
    return keys(colours);
}

void remove_colour(string which) {
    map_delete(colours, upper_case(which));
    /* just in case */
    map_delete(colours, lower_case(which));
    map_delete(colours, which);
    update_translations();
    save_me();
}
#endif

void do_receive(string msg, int msg_type) {
    if (msg_type & NO_ANSI) {
	if (msg_type & NO_WRAP)
	    receive(msg);
	else
	    receive(wrap(msg, query_screen_width()));
    } else {
	int indent = (msg_type & MSG_INDENT) ? 4 : 0;
	int wrap = (msg_type & NO_WRAP) ? 0 : query_screen_width();

	receive(terminal_colour(msg + "%^RESET%^", 
	    translations, wrap, indent));
    }
}

void receive_inside_msg(string msg, object * exclude, int message_type,
  mixed other)
{
    do_receive(msg, message_type);
}

void receive_outside_msg(string msg, object * exclude, int message_type,
  mixed other)
{
    do_receive(msg, message_type);
}

void receive_remote_msg(string msg, object * exclude, int message_type,
  mixed other)
{
    do_receive(msg, message_type);
}

void receive_private_msg(string msg, int message_type, mixed other)
{
    do_receive(msg, message_type);
}

