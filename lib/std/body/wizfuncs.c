/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wizfuncs.c -- wizard functions for the body
**
** 950820, Deathblade: created.
*/

object query_link();
string query_start_location();
varargs void receive_private_msg(string);

private int ilog_time;
private int ilog_off = 0;

int query_ilog_time()
{
    return ilog_time;
}

protected void set_ilog_time(int t)
{
    ilog_time = t;
}

// caller verification here
void ilog_hook(string str)
{
    if ( !ilog_off )
    {
      receive_private_msg("*** " + str + "\n");
      set_ilog_time(time());
    }
}

string query_home()
{
    string home;

    home = wiz_dir(this_object()) + "/workroom";
    if ( file_size(home + ".c") > 0 )
	return home;

    return "CLONE";
}

nomask void wiz_switch_body(string obname)
{
    query_link()->switch_body(obname, 0);

}

void set_ilog_off(int x)
{
  ilog_off = x;
}
