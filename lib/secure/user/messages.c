/* Do not remove the headers from this file! see /USAGE for more info. */

void receive_message(string msgclass, string msg)
{
    receive(msg);
}

void receive_inside_msg(string msg, object * exclude, int message_type,
			mixed other)
{
    receive(msg);
}

void receive_outside_msg(string msg, object * exclude, int message_type,
			 mixed other)
{
    receive(msg);
}

void receive_remote_msg(string msg, object * exclude, int message_type,
			mixed other)
{
    receive(msg);
}

void receive_private_msg(string msg, int message_type, mixed other)
{
    receive(msg);
}
