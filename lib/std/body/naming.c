/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** naming.c -- functions for naming and description of the player body
**
** 960603, Deathblade: created.
*/

#include <commands.h>		/* for CMD_OB_xxx */
#include <playerflags.h>        /* for F_INACTIVE */

private string describe;
private string invis_name;
private string nickname;

int is_visible();
int test_flag(int which);
int query_ghost();
object query_link();
string number_of(int num, string what);
void save_me();
string query_reflexive();
void remove_id(string array id...);
void add_id_no_plural(string array id...);
string in_room_desc();
string living_query_name();
string query_name();

#ifdef USE_TITLES
string query_title();
#endif


string query_long_name()
{
    if (query_ghost())
	return "The ghost of " + capitalize(living_query_name());
#ifdef USE_TITLES
    return query_title();
#else
    return capitalize(living_query_name());
#endif
}

nomask string query_userid()
{
  if(!query_link())
    return 0;
  return query_link()->query_userid();
}

nomask string query_invis_name()
{
    return invis_name;
}

string query_idle_string()
{
  object link = query_link();
  int idle_time;
  string result = "";

  if(test_flag(F_INACTIVE))
    result += " [INACTIVE] ";

  if ( interactive(link) )
    idle_time = query_idle(link);
  if ( idle_time < 60 )
    return result;

  result += " [idle " + convert_time(idle_time, 2) + "]";
  return result;
}

// This is used by in_room_desc and by who, one of which truncates,
// one of which doesnt.  Both want an idle time.
string base_in_room_desc()
{
    string result;
    object link = query_link();

    result = query_long_name();


    /* if they are link-dead, then prepend something... */
    if ( !link || !interactive(link) )
	result = "The lifeless body of " + result;

    return result;
}

string query_formatted_desc(int num_chars)
{
    string idle_string;
    int i;

    idle_string = query_idle_string();
    if ( i = strlen(idle_string) )
    {
	num_chars -= (i + 1);
	idle_string = " " + idle_string;
    }
    return M_ANSI->colour_truncate(base_in_room_desc(), num_chars) + idle_string;
}

string adjust_name(string name) {
    if ( invis_name == capitalize(name) || !invis_name ) invis_name = "Someone";
    if ( !is_visible() ) return invis_name;
    return capitalize(name);
}

void set_description(string str)
{
    if(base_name(previous_object()) == CMD_OB_DESCRIBE)
	describe = str;
    save_me();
}

string our_description()
{
    if (describe)
	return in_room_desc() + "\n" + describe +"\n";

    return in_room_desc() + "\n" + capitalize(query_name()) + " is boring and hasn't described " + query_reflexive() + ".\n";
}

void set_nickname(string arg)
{
    if (file_name(previous_object()) != CMD_OB_NICKNAME)
	error("Illegal call to set_nickname\n");

    if ( nickname )
	remove_id(nickname);

    nickname = arg;
    add_id_no_plural(nickname);
    parse_refresh();
}

string query_nickname()
{
    return nickname;
}

protected void naming_init_ids()
{
    if ( nickname )
	add_id_no_plural(nickname);
}
