/* Do not remove the headers from this file! see /USAGE for more info. */

/* %^RESET%^ gets used early in the login sequence by tell() */
private static mapping translations = (["RESET" : ""]);

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
    if (query_shell_ob()->get_variable("ansi"))
	translations = M_ANSI->query_translations()[0];
    else
	translations = M_ANSI->query_translations()[1];
}

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

